#include "test_generative_proof_machine.h"
// #include "physical_graph.h"
#include "generative_restriction.h"
#include "generative_proof_machine.h"
#include <assert.h>

void test_generative_proof_machine(void)
{
    printf("Small machine\n");
    execute_small_machine();
    printf("Creation and deletion\n");
    creation_and_deletion();
    printf("Max degree restriction\n");
    test_max_degree();
    printf("No ck restriction\n");
    test_no_ck_restriction();
    printf("No induced pk restriction\n");
    test_no_induced_pk_restriction();
    printf("Min degree\n");
    test_min_degree();
    /*
    printf("Check edge\n");
    test_check_edge();
    */
   printf("Erdos gyarfas subcase\n");
   test_erdos_gyarfas_subcase();
}

void creation_and_deletion(void)
{
    GenerativeRestriction *restrictions = create_max_degree_restriction(10);
    Graph *startGraph = create_graph(3,1);
    GenerativeProofMachine *machine = create_generative_proof_machine(&restrictions, 1, startGraph);

    destroy_generative_proof_machine(machine);
}

void test_max_degree(void)
{
    GenerativeRestriction *restrictions = create_max_degree_restriction(3);
    Graph *startGraph = create_graph(10,10);
    set_edge_connected(startGraph, 0, 1);
    set_edge_connected(startGraph, 0, 2);
    set_edge_connected(startGraph, 0, 3);
    GenerativeProofMachine *machine = create_generative_proof_machine(&restrictions, 1, startGraph);
    execute_generative_proof_machine(machine);
    char **adjMatrix = get_graph_adjacency_matrix(startGraph);
    for(int i = 4; i < 10; ++i)
    {
        assert(adjMatrix[0][i] == NOT_CONNECTED_SYMBOL);
    }
    for(int i = 1; i < 10; ++i)
    {
        for(int j = i+1; j < 10; ++j)
        {
            assert(adjMatrix[i][j] == UNKNOWN_SYMBOL);
        }
    }

    // adding edge should create contradiction
    set_edge_connected(startGraph, 0, 4);
    assert(execute_generative_proof_machine(machine) == 1);

    destroy_generative_proof_machine(machine);
}

void test_no_ck_restriction(void)
{
    // ====== test 1 ======
    GenerativeRestriction *restrictions = create_no_k_cycle_restriction(4);
    Graph *graph1 = create_graph(4, 4);
    set_edge_connected(graph1, 0, 1);
    set_edge_connected(graph1, 1, 2);
    set_edge_connected(graph1, 2, 3);
    PathNode *paths = find_k_paths(graph1, 4);
    int num_paths = 0;
    while(paths != NULL)
    {
        num_paths++;
        paths = paths->next;
    }
    assert(num_paths == 2);

    GenerativeProofMachine *machine = create_generative_proof_machine(&restrictions, 1, graph1);
    int contr1 = execute_generative_proof_machine(machine);
    assert(contr1 == 0);
    assert(get_graph_adjacency_matrix(graph1)[0][3] == NOT_CONNECTED_SYMBOL);

    set_edge_connected(graph1, 0, 3);
    int contr2 = execute_generative_proof_machine(machine);
    assert(contr2 == 1);

    // ====== test 2 ======

    Graph *graph2 = create_graph(15, 15);
    set_edge_connected(graph2, 0, 1);
    set_edge_connected(graph2, 0, 2);
    set_edge_connected(graph2, 1, 3);
    set_edge_connected(graph2, 1, 4);
    set_edge_connected(graph2, 2, 5);
    set_edge_connected(graph2, 2, 6);
    set_edge_connected(graph2, 3, 7);
    set_edge_connected(graph2, 3, 8);
    set_edge_connected(graph2, 4, 9);
    set_edge_connected(graph2, 4, 10);
    set_edge_connected(graph2, 6, 11);
    set_edge_connected(graph2, 6, 12);
    set_edge_connected(graph2, 7, 13);
    set_edge_connected(graph2, 8, 14);
    paths = find_k_paths(graph2, 2);
    num_paths = 0;
    while(paths != NULL)
    {
        num_paths++;
        paths = paths->next;
    }
    assert(num_paths == 28);

    // ===== test 3 =====

    Graph *graph3 = create_graph(50, 50);

    set_edge_connected(graph3, 0, 1);
    set_edge_connected(graph3, 1, 2);
    set_edge_connected(graph3, 2, 3);
    set_edge_connected(graph3, 3, 4);
    set_edge_connected(graph3, 4, 0);

    set_edge_connected(graph3, 1, 5);
    set_edge_connected(graph3, 5, 6);
    set_edge_connected(graph3, 6, 1);

    set_edge_connected(graph3, 2, 7);
    set_edge_connected(graph3, 7, 8);
    set_edge_connected(graph3, 8, 9);
    set_edge_connected(graph3, 9, 2);

    set_edge_connected(graph3, 3, 10);
    set_edge_connected(graph3, 10, 11);
    set_edge_connected(graph3, 11, 12);

    set_edge_connected(graph3, 4, 13);
    set_edge_connected(graph3, 13, 14);
    set_edge_connected(graph3, 14, 15);

    for (int i = 16; i < 50; i++) {
        set_edge_connected(graph3, i, i - 1);
    }

    set_edge_connected(graph3, 16, 10);
    set_edge_connected(graph3, 17, 5);
    set_edge_connected(graph3, 20, 9);
    set_edge_connected(graph3, 22, 0);

    restrictions = create_no_k_cycle_restriction(5);
    machine = create_generative_proof_machine(&restrictions, 1, graph3);
    int contr3 = execute_generative_proof_machine(machine);
    assert(contr3 == 1);

    // ===== test 4 =====
    Graph *graph4 = create_graph(50, 50);

    set_edge_connected(graph4, 0, 1);
    set_edge_connected(graph4, 1, 2);
    set_edge_connected(graph4, 2, 0);

    set_edge_connected(graph4, 3, 4);
    set_edge_connected(graph4, 4, 5);
    set_edge_connected(graph4, 5, 6);
    set_edge_connected(graph4, 6, 3);

    set_edge_connected(graph4, 7, 8);
    set_edge_connected(graph4, 8, 9);
    set_edge_connected(graph4, 9, 10);
    set_edge_connected(graph4, 10, 11);
    set_edge_connected(graph4, 11, 12);
    set_edge_connected(graph4, 12, 7);

    set_edge_connected(graph4, 14, 15);
    set_edge_connected(graph4, 15, 16);
    set_edge_connected(graph4, 16, 17);
    set_edge_connected(graph4, 17, 13);

    set_edge_connected(graph4, 18, 19);
    set_edge_connected(graph4, 19, 20);
    set_edge_connected(graph4, 20, 21);
    set_edge_connected(graph4, 21, 18);

    set_edge_connected(graph4, 22, 23);
    set_edge_connected(graph4, 23, 24);
    set_edge_connected(graph4, 24, 25);
    set_edge_connected(graph4, 25, 26);
    set_edge_connected(graph4, 26, 27);
    set_edge_connected(graph4, 27, 22);

    set_edge_connected(graph4, 2, 3);
    set_edge_connected(graph4, 6, 7);
    set_edge_connected(graph4, 12, 13);
    set_edge_connected(graph4, 17, 18);

    for (int i = 28; i < 50; i++) {
        set_edge_connected(graph4, i, i - 1);
    }
    set_edge_connected(graph4, 27, 28);

    set_edge_connected(graph4, 29, 1);
    set_edge_connected(graph4, 30, 4);
    set_edge_connected(graph4, 31, 9);
    set_edge_connected(graph4, 32, 20);

    restrictions = create_no_k_cycle_restriction(5);
    machine = create_generative_proof_machine(&restrictions, 1, graph4);
    int contr4 = execute_generative_proof_machine(machine);
    assert(contr4 == 0);

}

void execute_small_machine(void)
{
    
}

void execute_small_machine2(void)
{

}

void test_no_induced_pk_restriction(void)
{
    // ===== test 1 =====
    GenerativeRestriction *restrictions = create_no_induced_pk_restriction(3);
    Graph *startGraph = create_graph(10, 10);
    char **adjMatrix = get_graph_adjacency_matrix(startGraph);
    set_edge_connected(startGraph, 0, 1);
    set_edge_connected(startGraph, 1, 2);
    GenerativeProofMachine *machine = create_generative_proof_machine(&restrictions, 1, startGraph);
    int contr1 = execute_generative_proof_machine(machine);
    assert(contr1 == 0);
    assert(adjMatrix[0][1] == CONNECTED_SYMBOL);
    assert(adjMatrix[1][2] == CONNECTED_SYMBOL);
    assert(adjMatrix[0][2] == CONNECTED_SYMBOL);

    // ===== test 2 =====

    Graph *graph2 = create_graph(5, 5);
    adjMatrix = get_graph_adjacency_matrix(graph2);
    restrictions = create_no_induced_pk_restriction(5);
    machine = create_generative_proof_machine(&restrictions, 1, graph2);

    set_edge_connected(graph2, 0, 1);
    set_edge_connected(graph2, 1, 2);
    set_edge_connected(graph2, 2, 3);
    set_edge_connected(graph2, 3, 4);

    int contr2 = execute_generative_proof_machine(machine);
    assert(contr2 == 0);

    // 02 03 04 13 14 24
    set_edge_not_connected(graph2, 0, 2);
    set_edge_not_connected(graph2, 0, 3);
    set_edge_not_connected(graph2, 0, 4);
    set_edge_not_connected(graph2, 1, 3);
    set_edge_not_connected(graph2, 1, 4);
 
    contr2 = execute_generative_proof_machine(machine);
    assert(contr2 == 0);
    assert(adjMatrix[2][4] == CONNECTED_SYMBOL);

    // ===== test 3 =====

    // Arrange
    Graph *graph3 = create_graph(6, 6);
    GenerativeRestriction *restriction3 = create_no_induced_pk_restriction(2);
    GenerativeProofMachine *machine3 = create_generative_proof_machine(&restriction3, 1, graph3);
    char **adjMatrix3 = get_graph_adjacency_matrix(graph3);
    int contr3;
    int edgeCount = 0;

    // Act
    contr3 = execute_generative_proof_machine(machine3);
    for(int i = 0; i < 6; ++i)
    {
        for(int j = i+1; j < 6; ++j)
        {
            if(adjMatrix3[i][j] == CONNECTED_SYMBOL)
            {
                ++edgeCount;
            }
        }
    }

    // Assert
    assert(contr3 == 0);
    assert(edgeCount == 0);

    // ===== test 4 =====

    // Arrange
    Graph *graph4 = create_graph(6, 6);
    set_edge_connected(graph4, 2, 4);
    GenerativeRestriction *restriction4 = create_no_induced_pk_restriction(2);
    GenerativeProofMachine *machine4 = create_generative_proof_machine(&restriction4, 1, graph4);
    int contr4;

    // Act
    contr4 = execute_generative_proof_machine(machine4);

    // Assert
    assert(contr4 == 1);

    // (⊙_☉)
}

void test_check_edge(void)
{
    // ======= test 1 =======

    // Arrange


    // Act

    // Assert
}

void test_erdos_gyarfas_subcase(void)
{
    // ======= test 1 =======

    // Arrange
    // create induced C7
    Graph *graph1 = create_graph(15, 7);
    for(int i = 0; i < 7; ++i)
    {
        for(int j = 0; j < 7; ++j)
        {
            if(i == j)
            {
                continue;
            }
            if((i+1) % 7 == j || (i-1) % 7 == j)
            {
                set_edge_connected(graph1, i, j);
            }
            else
            {
                set_edge_not_connected(graph1, i, j);
            }
        }
    }
    GenerativeRestriction **restrictions1 = gtd_malloc(sizeof(GenerativeRestriction*) * 5);
    restrictions1[0] = create_no_k_cycle_restriction(4);
    restrictions1[1] = create_no_k_cycle_restriction(8);
    restrictions1[2] = create_no_induced_pk_restriction(8);
    restrictions1[3] = create_min_degree_restriction(3);
    GenerativeProofMachine *machine1 = create_generative_proof_machine(restrictions1, 5, graph1);
    restrictions1[4] = create_check_edge_restriction(3, machine1);

    // Act
    int contr1 = execute_generative_proof_machine(machine1);

    // Assert
    assert(contr1 == 1);
}

void test_min_degree(void)
{
    // ========== test 1 ==========

    // Arrange
    Graph *graph1 = create_graph(10, 1);
    GenerativeRestriction *restriction1 = create_min_degree_restriction(7);
    GenerativeProofMachine *machine1 = create_generative_proof_machine(&restriction1, 1, graph1);
    int *degree1 = get_graph_degree(graph1);

    // Act
    int contr1 = execute_generative_proof_machine(machine1);

    // Assert
    assert(contr1 == 0);
    assert(degree1[0] == 7);

    // ========== test 2 ==========

    // Arrange
    Graph *graph2 = create_graph(10, 3);
    GenerativeRestriction *restriction2 = create_min_degree_restriction(15);
    GenerativeProofMachine *machine2 = create_generative_proof_machine(&restriction2, 1, graph2);
    int *degree2 = get_graph_degree(graph2);

    // Act
    int contr2 = execute_generative_proof_machine(machine2);

    // Assert
    assert(contr2 == 0);
    assert(degree2[0] == 7);
}

