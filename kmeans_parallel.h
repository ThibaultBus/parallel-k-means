#ifndef KMEANS_PAR_H
#define KMEANS_PAR_H

#include "structs.h"

int kmeans_par(Point *points, Cluster *clusters, int nb_points, int nb_clusters, int max_iterations);

#endif /* KMEANS_PAR_H */
