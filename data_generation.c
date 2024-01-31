#include "data_generation.h"
#include <stdlib.h>

Cluster generate_secret_cluster(double coords_limit) {
    Cluster scluster;
    // Make a point around the center
    scluster.centroid_x = rand() % (int)coords_limit;
    scluster.centroid_y = rand() % (int)coords_limit;

    return scluster;
}

Point generate_point(double center_x, double center_y, int placement_range, double coords_limit) {
    Point point;
    // Make a point around the center
    do
    {
        point.x = center_x + (rand() % (placement_range * 2 + 1) - placement_range);
        point.y = center_y + (rand() % (placement_range * 2 + 1) - placement_range);
    } while (point.x < 0 || point.x > coords_limit || point.y < 0 || point.y > coords_limit);

    point.cluster_id = -1;
    return point;
}

/*
* Generate the points and clusters
*
* @param points: array of points
* @param secret_clusters: array of secret clusters
* @param nb_points: number of points
* @param nb_clusters: number of clusters
* @param canvas_size: size of the canvas (width = height)
* @param placement_range: range of the placement of the points around their secret cluster
*/
void generate_data(Point *points, Cluster *secret_clusters, int nb_points, int nb_clusters, int canvas_size, int placement_range) {
    // Generate clusters
    for (int i = 0; i < nb_clusters; i++) {
        secret_clusters[i] = generate_secret_cluster(canvas_size);
    }

    // Generate points
    for (int i = 0; i < nb_points; i++) {
        points[i] = generate_point(secret_clusters[rand() % nb_clusters].centroid_x, secret_clusters[rand() % nb_clusters].centroid_y, placement_range, canvas_size);
    }
}

Cluster generate_cluster(double coords_limit) {
    Cluster cluster;
    cluster.centroid_x = rand() % (int)coords_limit;
    cluster.centroid_y = rand() % (int)coords_limit;
    cluster.new_centroid_y = 0;
    cluster.new_centroid_y = 0;
    cluster.size = 0;
    return cluster;
}