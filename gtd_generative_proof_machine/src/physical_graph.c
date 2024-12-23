#include "physical_graph.h"
#define HASH_EXP 5


struct Graph
{
    int max_vertices;
    int vertices;
    char **adjacency_matrix;
    int *degree;
    // hash is sum of degree^HASH_EXP (not sum^HASH_EXP)
    int hash;

    // do we know for sure that from this graph
    // we can not generate any other graphs
    // not colliding with restrictions
    int is_maximal;
};

static int int_pow(int base, int exp);

Graph *create_graph(int max_vertices, int vertices)
{
    Graph *newGraph = (Graph *)gtd_malloc(sizeof(Graph));
    newGraph->hash = 0;
    newGraph->is_maximal = 0;
    newGraph->max_vertices = max_vertices;
    newGraph->vertices = vertices;
    newGraph->degree = (int *)gtd_malloc(sizeof(int) * max_vertices);
    newGraph->adjacency_matrix = (char **)gtd_malloc(sizeof(int*) * max_vertices);

    for(int i = 0; i < max_vertices; ++i)
    {
        newGraph->degree[i] = 0;
        newGraph->adjacency_matrix[i] = (char *)gtd_malloc(sizeof(char) * max_vertices);
        memset(newGraph->adjacency_matrix[i], 0, sizeof(char) * max_vertices);
    }

    for(int r = 0; r < max_vertices; ++r)
    {
        for(int c = 0; c < max_vertices; ++c)
        {
            newGraph->adjacency_matrix[r][c] = UNKNOWN_SYMBOL;
        }
    }

    return newGraph;
}

int destroy_graph(Graph *graph)
{
    gtd_free(graph->degree);
    for(int i = 0; i < graph->max_vertices; ++i)
    {
        gtd_free(graph->adjacency_matrix[i]);
    }
    gtd_free(graph->adjacency_matrix);

    return 1;
}

int get_graph_max_vertices(Graph *graph)
{
    return graph->max_vertices;
}

int get_graph_num_vertices(Graph *graph)
{
    return graph->vertices;
}

int get_graph_num_edges(Graph *graph)
{
    int result = 0;
    char **adjMatrix = get_graph_adjacency_matrix(graph);
    int n = get_graph_num_vertices(graph);
    for(int i = 0; i < n; ++i)
    {
        for(int j = i+1; i < n; ++j)
        {
            if(adjMatrix[i][j] == CONNECTED_SYMBOL)
            {
                ++result;
            }
        }
    }
    return result;
}

char **get_graph_adjacency_matrix(Graph *graph)
{
    return graph->adjacency_matrix;
}

int get_graph_hash(Graph *graph)
{
    return graph->hash / 2;
}

int set_graph_maximal(Graph *graph)
{
    graph->is_maximal = 1;
    return 1;
}

int get_graph_is_maximal(Graph *graph)
{
    return graph->is_maximal;
}

int set_edge_connected(Graph *graph, int from, int to)
{
    if(from < 0 || from >= graph->vertices || to < 0 || to >= graph->vertices
        || from == to)
    {
        return -1;
    }

    // update hash
    if(graph->adjacency_matrix[from][to] != CONNECTED_SYMBOL)
    {
        int from_degree = graph->degree[from];
        int to_degree = graph->degree[to];
        graph->hash += int_pow(from_degree + 1, HASH_EXP) + int_pow(to_degree + 1, HASH_EXP);
        graph->hash -= int_pow(from_degree, HASH_EXP) + int_pow(to_degree, HASH_EXP);
        graph->degree[from]++;
        graph->degree[to]++;
    }

    graph->adjacency_matrix[from][to] = CONNECTED_SYMBOL;
    graph->adjacency_matrix[to][from] = CONNECTED_SYMBOL;

    return 1;
}

int set_edge_not_connected(Graph *graph, int from, int to)
{
    if(from < 0 || from >= graph->vertices || to < 0 || to >= graph->vertices
        || from == to)
    {
        return -1;
    }

    if(graph->adjacency_matrix[from][to] == 1)
    {
        int from_degree = graph->degree[from];
        int to_degree = graph->degree[to];
        graph->hash += int_pow(from_degree - 1, HASH_EXP) + int_pow(to_degree - 1, HASH_EXP);
        graph->hash -= int_pow(from_degree, HASH_EXP) + int_pow(to_degree, HASH_EXP);
        if(graph->adjacency_matrix[from][to] == CONNECTED_SYMBOL)
        {
            graph->degree[from]--;
            graph->degree[to]--;
        }
    }

    graph->adjacency_matrix[from][to] = NOT_CONNECTED_SYMBOL;
    graph->adjacency_matrix[to][from] = NOT_CONNECTED_SYMBOL;

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

Graph *copy_graph(Graph *graph)
{
    Graph *newGraph = create_graph(graph->max_vertices, graph->vertices);
    newGraph->hash = graph->hash;
    int n = graph->max_vertices;
    for(int r = 0; r < n; ++r)
    {
        newGraph->degree[r] = graph->degree[r];
        for(int c = 0; c < n; ++c)
        {
            newGraph->adjacency_matrix[r][c] = graph->adjacency_matrix[r][c];
        }
    }
    return newGraph;
}

int *get_graph_degree(Graph *graph)
{
    return graph->degree;
}

// ================ ALGORITHMS ================

char **create_matrix(int rows, int cols)
{
    char **matrix = gtd_malloc(rows * sizeof(int*));

    for(int r = 0; r < rows; ++r)
    {
        matrix[r] = gtd_malloc(cols * sizeof(char));
    }

    return matrix;
}

static int int_pow(int base, int exp)
{
    int res = 1;
    for(int i = 0; i < exp; ++i)
    {
        res *= base;
    }
    return res;
}