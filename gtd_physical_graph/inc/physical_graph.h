#ifndef PHYSICAL_GRAPH_H
#define PHYSICAL_GRAPH_H
#include <stdint.h>
#include "common.h"
#include <string.h>

typedef struct Graph Graph;

Graph *create_graph(int max_vertices, int vertices);
// is pointer sufficient to destroy?
int destroy_graph(Graph *graph);
int get_graph_max_vertices(Graph *graph);
int get_graph_num_vertices(Graph *graph);
int **get_graph_adjacency_matrix(Graph *graph);
int add_edge(Graph *graph, int from, int to);
int remove_edge(Graph *graph, int from, int to);
int add_vertex(Graph *graph);
Graph *copyGraph(Graph *graph);

// create matrix filled with zeros
int **create_matrix(int rows, int cols);
// functions checks if 2 graphs are isomorphic
void swap(int *a, int *b);
int check_isomorphic(Graph *a, Graph *b);
int nextPermutation(int *arr, int n);
int get_graph_hash(Graph *graph);
int *get_graph_degree(Graph *graph);
int set_graph_maximal(Graph *graph);
int get_graph_is_maximal(Graph *graph);

int int_pow(int base, int exp);

#endif