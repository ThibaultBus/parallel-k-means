#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "structs.h"

double euclidean_distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void global_distance(Point *points, Cluster *clusters, int nb_points, int nb_clusters) {

    for (int i = 0; i < nb_points; i++) {
        double min_distance = euclidean_distance(points[i].x, points[i].y, clusters[0].centroid_x , clusters[0].centroid_y);
        int min_index = 0;

        for (int j = 1; j < nb_clusters; j++) {
            double distance = euclidean_distance(points[i].x, points[i].y, clusters[j].centroid_x, clusters[j].centroid_y);

            if (distance < min_distance) {
                min_distance = distance;
                min_index = j;
            }
        }
        points[i].cluster_id = min_index;
        // We add the coordinates of the point to the cluster to calculate the new centroid in update_clusters
        clusters[min_index].new_centroid_y += points[i].x;
        clusters[min_index].new_centroid_y += points[i].y;
        clusters[min_index].size++;
    }
}

/*
* Update the clusters' centroids
*
* @param clusters: array of clusters
* @param nb_clusters: number of clusters
*
* @return 1 if the clusters have converged (no change in the centroids), 0 otherwise
*/
int update_clusters(Cluster *clusters, int nb_clusters) {
    int converged = 1;

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
int kmeans_seq(Point *points, Cluster *clusters, int nb_points, int nb_clusters, int max_iterations) {
    int iterations = 0;
    int conv = 0;

    while (iterations < max_iterations && !conv) {
        iterations++;
        global_distance(points, clusters, nb_points, nb_clusters);
        conv = update_clusters(clusters, nb_clusters);
    }

    return iterations;
}