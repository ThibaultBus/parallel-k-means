#ifndef KMEANS_SEQ_H
#define KMEANS_SEQ_H

#include "structs.h"

int kmeans_seq(Point *points, Cluster *clusters, int nb_points, int nb_clusters, int max_iterations);

#endif /* KMEANS_SEQ_H */
