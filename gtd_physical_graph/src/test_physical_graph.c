#include "test_physical_graph.h"
#include "physical_graph.h"


void test_physical_graph(void)
{
    test_graph_creation_and_deletion();
    test_adding_vertices();
    test_adding_edges();
    test_isomorpic_pass();
    test_isomorpic_fail();
}

void test_graph_creation_and_deletion(void)
{
    Graph *graph = create_graph(20, 10);
    destroy_graph(graph);
}

void test_adding_vertices(void)
{
    Graph *graph = create_graph(10, 0);

    for(int i = 0; i < 5; ++i)
    {
        add_vertex(graph);
    }

    int num_vertices = get_graph_num_vertices(graph);
    assert(num_vertices == 5);

    destroy_graph(graph);
}

void test_adding_edges(void)
{
    Graph *graph = create_graph(10, 4);
    int n = get_graph_num_vertices(graph);

    int correctMatrix[4][4] = 
    {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    int **adjMatrix = get_graph_adjacency_matrix(graph);
    for(int r = 0; r < n; ++r)
    {
        for(int c = 0; c < n; ++c)
        {
            assert(adjMatrix[r][c] == correctMatrix[r][c]);
        }
    }

    add_edge(graph, 0, 2);
    add_edge(graph, 2, 3);
    add_edge(graph, 1, 3);


    int correctMatrix2[4][4] = 
    {
        {0, 0, 1, 0},
        {0, 0, 0, 1},
        {1, 0, 0, 1},
        {0, 1, 1, 0}
    };

    adjMatrix = get_graph_adjacency_matrix(graph);
    for(int r = 0; r < n; ++r)
    {
        for(int c = 0; c < n; ++c)
        {
            assert(adjMatrix[r][c] == correctMatrix2[r][c]);
        }
    }

    destroy_graph(graph);
}

void test_isomorpic_pass(void)
{
    Graph *g1 = create_graph(5, 5);
    add_edge(g1, 0, 1);
    add_edge(g1, 0, 4);
    add_edge(g1, 1, 2);
    add_edge(g1, 2, 3);
    add_edge(g1, 3, 4);

    Graph *g2 = create_graph(5, 5);
    add_edge(g2, 0, 1);
    add_edge(g2, 0, 3);
    add_edge(g2, 1, 4);
    add_edge(g2, 2, 3);
    add_edge(g2, 2, 4);

    int res = check_isomorphic(g1, g2);

    assert(res == 1);

    destroy_graph(g1);
    destroy_graph(g2);
}

void test_isomorpic_fail(void)
{Graph *g1 = create_graph(5, 5);
    add_edge(g1, 0, 1);
    add_edge(g1, 0, 4);
    add_edge(g1, 1, 2);
    add_edge(g1, 1, 3); // adding this one should fail
    add_edge(g1, 2, 3);
    add_edge(g1, 3, 4);

    Graph *g2 = create_graph(5, 5);
    add_edge(g2, 0, 1);
    add_edge(g2, 0, 3);
    add_edge(g2, 1, 4);
    add_edge(g2, 2, 3);
    add_edge(g2, 2, 4);

    int res = check_isomorphic(g1, g2);

    assert(res == 0);

    destroy_graph(g1);
    destroy_graph(g2);

}