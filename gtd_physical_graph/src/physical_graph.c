#include "physical_graph.h"

struct Graph
{
    int max_vertices;
    int vertices;
    int **adjacency_matrix;
    int *degree;
    // hash is sum of degree^3 (not sum^3)
    int hash;
};

Graph *create_graph(int max_vertices, int vertices)
{
    Graph *newGraph = (Graph *)gtd_malloc(sizeof(Graph));
    newGraph->hash = 0;
    newGraph->max_vertices = max_vertices;
    newGraph->vertices = vertices;
    newGraph->degree = (int *)gtd_malloc(sizeof(int) * max_vertices);
    newGraph->adjacency_matrix = (int **)gtd_malloc(sizeof(int*) * max_vertices);

    for(int i = 0; i < max_vertices; ++i)
    {
        newGraph->degree[i] = 0;
        newGraph->adjacency_matrix[i] = (int *)gtd_malloc(sizeof(int) * max_vertices);
        memset(newGraph->adjacency_matrix[i], 0, sizeof(int) * max_vertices);
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

int **get_graph_adjacency_matrix(Graph *graph)
{
    return graph->adjacency_matrix;
}

int get_graph_hash(Graph *graph)
{
    return graph->hash / 2;
}

int add_edge(Graph *graph, int from, int to)
{
    if(from < 0 || from >= graph->vertices || to < 0 || to >= graph->vertices)
    {
        return -1;
    }

    if(graph->adjacency_matrix[from][to] == 0)
    {
        int from_degree = graph->degree[from];
        int to_degree = graph->degree[to];
        graph->hash += int_pow(from_degree + 1, 3) + int_pow(to_degree + 1, 3);
        graph->hash -= int_pow(from_degree, 3) + int_pow(to_degree, 3);
        graph->degree[from]++;
        graph->degree[to]++;
    }

    graph->adjacency_matrix[from][to] = 1;
    graph->adjacency_matrix[to][from] = 1;

    return 1;
}

int remove_edge(Graph *graph, int from, int to)
{
    if(from < 0 || from >= graph->vertices || to < 0 || to >= graph->vertices)
    {
        return -1;
    }

    if(graph->adjacency_matrix[from][to] == 1)
    {
        int from_degree = graph->degree[from];
        int to_degree = graph->degree[to];
        graph->hash += int_pow(from_degree - 1, 3) + int_pow(to_degree - 1, 3);
        graph->hash -= int_pow(from_degree, 3) + int_pow(to_degree, 3);
        graph->degree[from]--;
        graph->degree[to]--;
    }

    graph->adjacency_matrix[from][to] = 0;
    graph->adjacency_matrix[to][from] = 0;

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

Graph *copyGraph(Graph *graph)
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

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int **create_matrix(int rows, int cols)
{
    int **matrix = gtd_malloc(rows * sizeof(int*));

    for(int r = 0; r < rows; ++r)
    {
        matrix[r] = gtd_malloc(cols * sizeof(int));
    }

    return matrix;
}

int check_isomorphic(Graph *g1, Graph *g2)
{
    // Step 1: Check if the number of vertices is the same
    //if (g1->vertices != g2->vertices || g1->hash != g2->hash)
    if(g1->vertices != g2->vertices)
    {
        return 0;
    }

    int n = g1->vertices;

    // Step 2: Generate all possible permutations of the vertices
    int *perm = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        perm[i] = i; // Initialize permutation as [0, 1, 2, ..., n-1]
    }

    do
    {
        int is_isomorphic = 1;

        for (int i = 0; i < n && is_isomorphic; i++)
        {
            for (int j = 0; j < n && is_isomorphic; j++)
            {
                // Check if g1[perm[i]][perm[j]] == g2[i][j]
                if (g1->adjacency_matrix[perm[i]][perm[j]] != g2->adjacency_matrix[i][j])
                {
                    is_isomorphic = 0;
                }
            }
        }

        if (is_isomorphic)
        {
            free(perm);
            return 1; // Graphs are isomorphic
        }

    } while (nextPermutation(perm, n)); // Generate next permutation

    free(perm);
    return 0;

}

int nextPermutation(int *arr, int n)
{
    int i = n - 2;
    // Find the largest i such that arr[i] < arr[i + 1]
    while (i >= 0 && arr[i] >= arr[i + 1])
    {
        i--;
    }
    if (i < 0)
    {
        return 0; // No more permutations
    }

    // Find the largest j such that arr[j] > arr[i]
    int j = n - 1;
    while (arr[j] <= arr[i])
    {
        j--;
    }

    // Swap arr[i] and arr[j]
    swap(&arr[i], &arr[j]);

    // Reverse the elements after i
    int k = i + 1, l = n - 1;
    while (k < l)
    {
        swap(&arr[k], &arr[l]);
        k++;
        l--;
    }

    return 1;
}

int int_pow(int base, int exp)
{
    int res = 1;
    for(int i = 0; i < exp; ++i)
    {
        res *= base;
    }
    return res;
}