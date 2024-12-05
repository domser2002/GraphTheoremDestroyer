#ifndef PHYSICAL_GRAPH_H
#define PHYSICAL_GRAPH_H
#include <stdint.h>
#include "common.h"
#include <string.h>
#define CONNECTED_SYMBOL 'C'
#define NOT_CONNECTED_SYMBOL 'N'
#define UNKNOWN_SYMBOL 'U'

typedef struct Graph Graph;

Graph *create_graph(int max_vertices, int vertices);
int destroy_graph(Graph *graph);
int get_graph_max_vertices(Graph *graph);
int get_graph_num_vertices(Graph *graph);
char **get_graph_adjacency_matrix(Graph *graph);
int set_edge_connected(Graph *graph, int from, int to);
int set_edge_not_connected(Graph *graph, int from, int to);
int add_vertex(Graph *graph);
Graph *copy_graph(Graph *graph);

// create matrix filled with unknowns
char **create_matrix(int rows, int cols);
int get_graph_hash(Graph *graph);
int *get_graph_degree(Graph *graph);
int set_graph_maximal(Graph *graph);
int get_graph_is_maximal(Graph *graph);

int int_pow(int base, int exp);

#endif