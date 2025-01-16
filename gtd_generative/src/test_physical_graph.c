/**
 * \file test_physical_graph.c
 * \brief implementation of functions from test_physical_graph.h
 */
#define IS_GENERATIVE_MODULE_COMPONENT
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
    // ===== TEST 1 =====
    Graph *graph = create_graph(10, 0);

    for(int i = 0; i < 5; ++i)
    {
        add_vertex(graph);
    }

    int num_vertices = get_graph_num_vertices(graph);
    assert(num_vertices == 5);

    destroy_graph(graph);

    // ===== TEST 2 =====
    Graph *graph2 = create_graph(5, 3);
    int canAdd1 = add_vertex(graph2);
    int size1 = get_graph_num_vertices(graph2);
    int canAdd2 = add_vertex(graph2);
    int size2 = get_graph_num_vertices(graph2);
    int canAdd3 = add_vertex(graph2);
    int size3 = get_graph_num_vertices(graph2);

    assert(canAdd1 == 1);
    assert(size1 == 4);
    assert(canAdd2 == 1);
    assert(size2 == 5);
    assert(canAdd3 == -1);
    assert(size3 == 5);

    destroy_graph(graph2);

    // ===== TEST 3 =====
    Graph *graph3 = create_graph(-1, 1);
    for(int i = 2; i < 100; ++i)
    {
        int canAdd = add_vertex(graph3);
        int size = get_graph_num_vertices(graph3);
        assert(canAdd == 1);
        assert(size == i);
    }

    destroy_graph(graph3);
}

void test_adding_edges(void)
{
    Graph *graph = create_graph(10, 0);

    for(int i = 0; i < 8; ++i)
    {
        add_vertex(graph);
    }

    int num_vertices = get_graph_num_vertices(graph);
    char **adjMatrix = get_graph_adjacency_matrix(graph);

    set_edge_connected(graph, 3, 5);
    set_edge_connected(graph, 4, 6);
    set_edge_connected(graph, 1, 5);
    
    assert(adjMatrix[3][5] == CONNECTED_SYMBOL);
    assert(adjMatrix[4][6] == CONNECTED_SYMBOL);
    assert(adjMatrix[1][5] == CONNECTED_SYMBOL);


    set_edge_not_connected(graph, 0, 1);
    set_edge_not_connected(graph, 4, 6);

    assert(adjMatrix[0][1] == NOT_CONNECTED_SYMBOL);
    assert(adjMatrix[4][6] == NOT_CONNECTED_SYMBOL);

    for(int from = 0; from < num_vertices; ++from)
    {
        for(int to = 0; to < num_vertices; ++to)
        {
            if((from == 3 && to == 5) || (from == 5 && to == 3))
                continue;
            if((from == 4 && to == 6) || (from == 6 && to == 4))
                continue;
            if((from == 5 && to == 1) || (from == 1 && to == 5))
                continue;
            if((from == 0 && to == 1) || (from == 1 && to == 0))
                continue;
            
            assert(adjMatrix[from][to] == UNKNOWN_SYMBOL);
        }
    }
    

    destroy_graph(graph);
}

void test_isomorpic_pass(void)
{
    
}

void test_isomorpic_fail(void)
{
    

}