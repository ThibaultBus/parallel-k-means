#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "structs.h"

double euclidean_distance_par(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void global_distance_par(Point *points, Cluster *clusters, int nb_points, int nb_clusters) {

    /*omp_lock_t lock[nb_clusters];

    // Initialize the locks for the clusters
    for (int i = 0; i < nb_clusters; i++) {
        omp_init_lock(&lock[i]);
    }*/

    #pragma omp parallel shared(points, clusters/*, lock*/) firstprivate(nb_points, nb_clusters) default(none)
    {
        // The calculations take the exact same time at every loop, so we can use static scheduling
        #pragma omp for schedule(static)
        for (int i = 0; i < nb_points; i++) {
            double min_distance = euclidean_distance_par(points[i].x, points[i].y, clusters[0].centroid_x , clusters[0].centroid_y);
            int min_index = 0;

            for (int j = 1; j < nb_clusters; j++) {
                double distance = euclidean_distance_par(points[i].x, points[i].y, clusters[j].centroid_x, clusters[j].centroid_y);

                if (distance < min_distance) {
                    min_distance = distance;
                    min_index = j;
                }
            }

            points[i].cluster_id = min_index;

            // Lock the cluster closest to the point
            //omp_set_lock(&lock[min_index]);

            // We add the coordinates of the point to the cluster to calculate the new centroid in update_clusters_par
            // As these are simple additions, we can use atomic operations to optimize it
            # pragma omp atomic
            clusters[min_index].new_centroid_y += points[i].x;
            # pragma omp atomic
            clusters[min_index].new_centroid_y += points[i].y;
            # pragma omp atomic
            clusters[min_index].size++;

            //omp_unset_lock(&lock[min_index]);
        }
    }

    // Destroy the locks for the clusters
    /*for (int i = 0; i < nb_clusters; i++) {
        omp_destroy_lock(&lock[i]);
    }*/
}

/*
* Update the clusters' centroids
*
* @param clusters: array of clusters
* @param nb_clusters: number of clusters
*
* @return 1 if the clusters have converged (no change in the centroids), 0 otherwise
*/
int update_clusters_par(Cluster *clusters, int nb_clusters) {
    int converged = 1;
    // really worth it ? probably not since there's very few calculations
    //# pragma omp parallel for reduction(&&:converged)
    for (int i = 0; i < nb_clusters; i++) {
        if (clusters[i].size > 0) {
            double new_x = clusters[i].new_centroid_y / clusters[i].size;
            double new_y = clusters[i].new_centroid_y / clusters[i].size;

            // If there is no change in the centroid, we don't need to continue
            if (clusters[i].centroid_x == new_x && clusters[i].centroid_y == new_y)
                continue;

            clusters[i].centroid_x = new_x;
            clusters[i].centroid_y = new_y;
            clusters[i].new_centroid_y = 0;
            clusters[i].new_centroid_y = 0;
            clusters[i].size = 0;
            converged = 0;
        }
    }

    return converged;
}

/*
* Sequential K-means clustering function
*
* @param points: array of points
* @param clusters: array of clusters
* @param nb_points: number of points
* @param nb_clusters: number of clusters
* @param max_iterations: maximum number of iterations
* @param canvas_size: size of the canvas (width = height)
*
* @return number of iterations actually done
*/
int kmeans_par(Point *points, Cluster *clusters, int nb_points, int nb_clusters, int max_iterations) {
    int iterations = 0;
    int conv = 0;

    while (iterations < max_iterations && !conv) {
        iterations++;
        global_distance_par(points, clusters, nb_points, nb_clusters);
        conv = update_clusters_par(clusters, nb_clusters);
    }

    return iterations;
}