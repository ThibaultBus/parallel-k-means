
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "structs.h"
#include "data_generation.h"
#include "kmeans_sequential.h"
#include "kmeans_parallel.h"

// -1 to disable all text except numerical results, 0 to enable normal text, 1 to enable all details verbose
#define VERBOSE -1

#define TOTAL_ITERATIONS 5

#define CANVAS_SIZE 10000

// Used for random generation of points
#define NB_POINTS 200000
#define NB_SECRET_CLUSTERS 200 // should be equal to nb_clusters
#define SECRET_CLUSTERS_SIZE 2000

// Used for k-means
#define NB_CLUSTERS 200
#define MAX_ITERATIONS 20 // should never be reached


void kmeans_comparison(int canvas_size, int nb_points, int nb_secret_clusters, int secret_clusters_size, int nb_clusters, int max_iterations, double* results) {
    #if VERBOSE != -1
    printf("Generating testing data..\n");
    #endif

    Point *points = (Point *)malloc(nb_points * sizeof(Point));
    Cluster *secret_clusters = (Cluster *)malloc(nb_secret_clusters * sizeof(Cluster));
    
    generate_data(points, secret_clusters, nb_points, nb_secret_clusters, canvas_size, secret_clusters_size);

    // Generate clusters and copy them to clusters_seq and clusters_par

    Cluster *clusters_seq = (Cluster *)malloc(nb_clusters * sizeof(Cluster));
    Cluster *clusters_par = (Cluster *)malloc(nb_clusters * sizeof(Cluster));

    for (int i = 0; i < nb_clusters; i++) {
        Cluster cluster = generate_cluster(canvas_size);
        clusters_seq[i] = cluster;
        clusters_par[i] = cluster;
    }

    #if VERBOSE != -1
    printf("Testing data generated.\n");
    #endif

    // Sequential k-means
    double start = omp_get_wtime(); // we use omp_get_wtime() instead of clock() because it doesn't work with OpenMP as it measures the time of all threads
    int seq_iterations = kmeans_seq(points, clusters_seq, nb_points, nb_clusters, max_iterations);
    double end = omp_get_wtime();

    double seq_time = (double)(end - start) * 1000.0;

    #if VERBOSE != -1
    printf("Sequential k-means took %f milliseconds.\n", seq_time;
    #endif

    // Parallel k-means
    start = omp_get_wtime();
    int par_iterations = kmeans_par(points, clusters_par, nb_points, nb_clusters, max_iterations);    
    end = omp_get_wtime();

    double par_time = (double)(end - start) * 1000.0;

    #if VERBOSE != -1
    printf("Parallel k-means took %f milliseconds.\n", par_time);
    #endif

    if (results != NULL) {
        results[0] = seq_time;
        results[1] = par_time;
    }

#if VERBOSE == 1
    printf("Secret clusters:\n");

    for (int i = 0; i < nb_secret_clusters; i++) {
        printf("Secret Cluster %d: (%f, %f)\n", i, secret_clusters[i].centroid_x, secret_clusters[i].centroid_y);
    }

    printf("Sequential k-means:\n");
    printf("Iterations: %d\n", seq_iterations);

    for (int i = 0; i < nb_clusters; i++) {
        printf("Cluster %d: (%f, %f)\n", i, clusters_seq[i].centroid_x, clusters_seq[i].centroid_y);
    }

    printf("Parallel k-means:\n");
    printf("Iterations: %d\n", par_iterations);

    for (int i = 0; i < nb_clusters; i++) {
        printf("Cluster %d: (%f, %f)\n", i, clusters_par[i].centroid_x, clusters_par[i].centroid_y);
    }
#endif

    free(points);
    free(clusters_seq);
    free(clusters_par);
    free(secret_clusters);
}

int main() {
    srand(time(NULL));

    // testing with different number of clusters

    for (int nb_clusters = 25; nb_clusters < 500; nb_clusters += 25)
    {
        double avg_results[2] = {0, 0};

        for (int iterations = 0; iterations < TOTAL_ITERATIONS; iterations++)
        {
            double results[2];
            #if VERBOSE != -1
            printf("Testing with %d clusters..\n", nb_clusters);
            #endif
            kmeans_comparison(CANVAS_SIZE, NB_POINTS, NB_SECRET_CLUSTERS, SECRET_CLUSTERS_SIZE, nb_clusters, MAX_ITERATIONS, results);

            avg_results[0] += results[0];
            avg_results[1] += results[1];
        }

        avg_results[0] /= TOTAL_ITERATIONS;
        avg_results[1] /= TOTAL_ITERATIONS;

        #if VERBOSE == -1
        printf("%d,%f,%f\n", nb_clusters, avg_results[0], avg_results[1]);
        #endif        
    }
    

    return 0;
}