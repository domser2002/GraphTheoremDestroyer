#ifndef PHYSICAL_GRAPH_H 
#define PHYSICAL_GRAPH_H
#include <stdint.h>
#include "common.h"
#include <string.h>
#include <math.h>
#define CONNECTED_SYMBOL 'C'
#define NOT_CONNECTED_SYMBOL 'N'
#define UNKNOWN_SYMBOL 'U'

typedef struct Graph Graph;

/**
 * \brief Function to create a graph
 * \param max_vertices int - max number of vertices in a graph, -1 if graph can be any size
 * \param vertices int - number of vertices that graph will be created with
 * \return pointer to the Graph structure
 */
Graph *create_graph(int max_vertices, int vertices);

/**
 * \brief Function to free all memory associated with a graph
 * \param graph - Graph which memory will be freed
 * \return 1 if succeded, -1 otherwise
 */
int destroy_graph(Graph *graph);

/**
 * \brief getter function for max_vertices field in a Graph structure
 * \param graph - pointer to the Graph structure
 * \return int - max number of vertices of a graph
 */
int get_graph_max_vertices(Graph *graph);

/**
 * \brief getter function for num_vertices field in a Graph structure
 * \param graph - pointer to the Graph structure
 * \return int - number of vertices in a graph
 */
int get_graph_num_vertices(Graph *graph);

/**
 * \brief Function to access adjacency matrix of a graph
 * \param graph - pointer to the Graph structure
 * \return char** - adjacency matrix of a graph
 */
char **get_graph_adjacency_matrix(Graph *graph);

/**
 * \brief Function to set edge state to connected
 * \param graph - pointer to the Graph structure
 * \param from - int describing first vertex in an edge
 * \param to - int describing second vertex in an edge
 * \return int - 1 if state of an edge (from to) was set to connected, -1 otherwise
 */
int set_edge_connected(Graph *graph, int from, int to);

/**
 * \brief Function to set edge state to not connected
 * \param graph - pointer to the Graph structure
 * \param from - int describing first vertex in an edge
 * \param to - int describing second vertex in an edge
 * \return int - 1 if state of an edge (from to) was set to not connected, -1 otherwise
 */
int set_edge_not_connected(Graph *graph, int from, int to);

/**
 * \brief Function to add new vertex to the graph
 * \param graph - pointer to the graph structure
 * \return int - 1 if new vertex succesfuly added, -1 otherwise
 */
int add_vertex(Graph *graph);

/**
 * \brief Function to create a deep copy of a graph
 * \param graph - pointer to the graph structure
 * \return Graph* - pointer to the newly created deep copy of graph
 */
Graph *copy_graph(Graph *graph);

// create matrix filled with unknowns
char **create_matrix(int rows, int cols);
int *get_graph_degree(Graph *graph);
int set_graph_maximal(Graph *graph);
int get_graph_is_maximal(Graph *graph);

#endif