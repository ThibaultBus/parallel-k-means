#ifndef DATA_GENERATION_H
#define DATA_GENERATION_H

#include "structs.h"

void generate_data(Point *points, Cluster *secret_clusters, int nb_points, int nb_clusters, int canvas_size, int placement_range);
Cluster generate_cluster(double coords_limit);

#endif /* DATA_GENERATION_H */