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
    newGraph->degree = (int *)gtd_malloc(sizeof(int) * vertices);
    newGraph->adjacency_matrix = (char **)gtd_malloc(sizeof(int*) * vertices);

    for(int i = 0; i < vertices; ++i)
    {
        newGraph->degree[i] = 0;
        newGraph->adjacency_matrix[i] = (char *)gtd_malloc(sizeof(char) * vertices);
        memset(newGraph->adjacency_matrix[i], 0, sizeof(char) * vertices);
    }

    for(int r = 0; r < vertices; ++r)
    {
        for(int c = 0; c < vertices; ++c)
        {
            newGraph->adjacency_matrix[r][c] = UNKNOWN_SYMBOL;
        }
    }

    return newGraph;
}

int destroy_graph(Graph *graph)
{
    if(graph == NULL)
        return 1;
    gtd_free(graph->degree);
    for(int i = 0; i < graph->vertices; ++i)
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
    char **newMatrix = gtd_realloc(
        graph->adjacency_matrix,
        sizeof(char*) * graph->vertices
        );
    graph->adjacency_matrix = newMatrix;
    
    for(int i = 0; i < (graph->vertices)-1; ++i)
    {
        char *newRow = gtd_realloc(graph->adjacency_matrix[i], sizeof(char) * graph->vertices);
        graph->adjacency_matrix[i] = newRow;
    }
    graph->adjacency_matrix[(graph->vertices) - 1] = gtd_malloc(sizeof(char) * (graph->vertices));

    for(int i = 0; i < graph->vertices; ++i)
    {
        graph->adjacency_matrix[i][(graph->vertices) - 1] = UNKNOWN_SYMBOL;
        graph->adjacency_matrix[(graph->vertices) - 1][i] = UNKNOWN_SYMBOL;
    }

    int *newDegree = gtd_realloc(graph->degree, sizeof(int) * (graph->vertices));
    newDegree[(graph->vertices) - 1] = 0;
    graph->degree = newDegree;

    return 1;
}

Graph *copy_graph(Graph *graph)
{
    if(graph == NULL)
        return NULL;
    Graph *newGraph = create_graph(graph->max_vertices, graph->vertices);
    newGraph->hash = graph->hash;
    int n = graph->vertices;
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