#ifndef PHYSICAL_GRAPH_H
#define PHYSICAL_GRAPH_H
#include <stdint.h>
#include "common.h"
#include <string.h>

typedef struct Graph Graph;

static Graph *create_graph(int max_vertices, int vertices);
int add_edge(Graph *graph, int from, int to);
int remove_edge(Graph *graph, int from, int to);
int add_vertex(Graph *graph);

#endif