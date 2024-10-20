#include "physical_graph.h"

struct Graph
{
    int max_vertices;
    int vertices;
    int **adjacency_matrix;
};

static Graph *create_graph(int max_vertices, int vertices)
{
    Graph *newGraph = (Graph *)gtd_malloc(sizeof(Graph));
    newGraph->max_vertices = max_vertices;
    newGraph->vertices = vertices;
    newGraph->adjacency_matrix = (int **)gtd_malloc(sizeof(int*) * max_vertices);

    for(int i = 0; i < max_vertices; ++i)
    {
        newGraph->adjacency_matrix[i] = (int *)gtd_malloc(sizeof(int) * max_vertices);
        memset(newGraph->adjacency_matrix[i], 0, sizeof(int) * max_vertices);
    }

    return newGraph;
}

int add_edge(Graph *graph, int from, int to)
{
    if(from < 0 || from >= graph->vertices || to < 0 || to >= graph->vertices)
    {
        return -1;
    }

    graph->adjacency_matrix[from, to] = 1;
    graph->adjacency_matrix[to, from] = 1;

    return 1;
}

int remove_edge(Graph *graph, int from, int to)
{
    if(from < 0 || from >= graph->vertices || to < 0 || to >= graph->vertices)
    {
        return -1;
    }

    graph->adjacency_matrix[from, to] = 0;
    graph->adjacency_matrix[to, from] = 0;

    return 1;
}

int add_vertex(Graph *graph)
{
    if(graph->vertices == graph->max_vertices)
    {
        return -1;
    }

    graph->vertices++;

    return 1;
}