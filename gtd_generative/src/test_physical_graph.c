#include "test_physical_graph.h"
#include "physical_graph.h"


void test_physical_graph(void)
{
    test_graph_creation_and_deletion();
    test_adding_vertices();
    test_adding_edges();
    test_get_graph_max_vertices();
    test_get_graph_num_vertices();
    test_get_graph_adjacency_matrix();
}

void test_graph_creation_and_deletion(void)
{
    // Arrange
    Graph *graph = create_graph(20, 10);

    // Act
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

void test_get_graph_max_vertices(void)
{
    // test 1
    // Arrange
    Graph *g1 = create_graph(15, 9);
    int test_1_max_1;
    int test_1_max_2;
    int test_1_max_3;
    int test_1_max_4;
    int test_1_max_5;

    // Act
    test_1_max_1 = get_graph_max_vertices(g1);
    add_vertex(g1);
    add_vertex(g1);
    add_vertex(g1);
    test_1_max_2 = get_graph_max_vertices(g1);
    add_vertex(g1);
    add_vertex(g1);
    add_vertex(g1);
    test_1_max_3 = get_graph_max_vertices(g1);
    add_vertex(g1);
    test_1_max_4 = get_graph_max_vertices(g1);
    add_vertex(g1);
    add_vertex(g1);
    add_vertex(g1);
    test_1_max_5 = get_graph_max_vertices(g1);

    // Assert
    assert(test_1_max_1 == 15);
    assert(test_1_max_2 == 15);
    assert(test_1_max_3 == 15);
    assert(test_1_max_4 == 15);
    assert(test_1_max_5 == 15);


    // test 2
    // Arrange
    Graph *g2 = create_graph(-1, 5);
    int test_2_max_1;
    int test_2_max_2;

    // Act
    test_2_max_1 = get_graph_max_vertices(g2);
    for(int i = 0; i < 100; ++i)
    {
        add_vertex(g2);
    }
    test_2_max_2 = get_graph_max_vertices(g2);

    // Assert
    assert(test_2_max_1 == -1);
    assert(test_2_max_2 == -1);
}

void test_get_graph_num_vertices(void)
{
    // test 1
    // Arrange
    Graph *g1 = create_graph(15, 9);
    int test_1_num_1;
    int test_1_num_2;
    int test_1_num_3;
    int test_1_num_4;
    int test_1_num_5;

    // Act
    test_1_num_1 = get_graph_num_vertices(g1);
    add_vertex(g1);
    add_vertex(g1);
    add_vertex(g1);
    test_1_num_2 = get_graph_num_vertices(g1);
    add_vertex(g1);
    add_vertex(g1);
    add_vertex(g1);
    test_1_num_3 = get_graph_num_vertices(g1);
    add_vertex(g1);
    test_1_num_4 = get_graph_num_vertices(g1);
    add_vertex(g1);
    add_vertex(g1);
    add_vertex(g1);
    test_1_num_5 = get_graph_num_vertices(g1);

    // Assert
    assert(test_1_num_1 == 9);
    assert(test_1_num_2 == 12);
    assert(test_1_num_3 == 15);
    assert(test_1_num_4 == 15);
    assert(test_1_num_5 == 15);


    // test 2
    // Arrange
    Graph *g2 = create_graph(-1, 5);
    int test_2_num_1;
    int test_2_num_2;

    // Act
    test_2_num_1 = get_graph_num_vertices(g2);
    for(int i = 0; i < 100; ++i)
    {
        add_vertex(g2);
    }
    test_2_num_2 = get_graph_num_vertices(g2);

    // Assert
    assert(test_2_num_1 == 5);
    assert(test_2_num_2 == 105);
}

void test_get_graph_adjacency_matrix(void)
{
    // test 1
    // Arrange
    Graph *g1 = create_graph(5, 5);
    char **adjMatrix1 = get_graph_adjacency_matrix(g1);

    // Act/Assert
    for(int i = 0; i < 5; ++i)
    {
        for(int j = 0; j < 5; ++j)
        {
            assert(adjMatrix1[i][j] == UNKNOWN_SYMBOL);
        }
    }

    set_edge_connected(g1, 0, 3);
    set_edge_connected(g1, 3, 2);
    set_edge_not_connected(g1, 4, 0);
    assert(adjMatrix1[0][3] == CONNECTED_SYMBOL);
    assert(adjMatrix1[3][2] == CONNECTED_SYMBOL);
    assert(adjMatrix1[0][4] == NOT_CONNECTED_SYMBOL);
}