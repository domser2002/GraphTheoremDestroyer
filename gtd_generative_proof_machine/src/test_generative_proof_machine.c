#define IS_GENERATIVE_MODULE_COMPONENT
#include "test_generative_proof_machine.h"
#include "generative_restriction.h"
#include "generative_proof_machine.h"
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

void test_generative_proof_machine(void)
{
    printf("Creation and deletion\n");
    test_machine_creation_and_deletion();

    printf("Max degree restriction\n");
    test_max_degree_restriction();

    printf("No ck restriction\n");
    test_no_k_cycle_restriction();

    printf("No induced pk restriction\n");
    test_no_induced_path_k_restriction();

    printf("Min degree\n");
    test_minimum_degree_restriction();

    printf("Erdos gyarfas subcase\n");
    test_erdos_gyarfas_case();

    printf("Erdos gyarfas p7 free\n");
    test_erdos_gyarfas_pk_free(7, 15, 2, 1);

    printf("Edros gyarfas p8 free\n");
    test_erdos_gyarfas_pk_free(8, 40, 5, 1);

    // uncomment below lines for more erdos-gyarfas cases
    /*
    printf("Erdos gyarfas p9 free\n");
    test_erdos_gyarfas_pk_free(9, 25, 3, 1);

    printf("Erdos gyarfas p10 free\n");
    test_erdos_gyarfas_pk_free(10, 50, 5, 1);

    printf("Erdos gyarfas p11 free\n");
    test_erdos_gyarfas_pk_free(11, 50, 5, 1);

    printf("Erdos gyarfas p12 free\n");
    test_erdos_gyarfas_pk_free(12, 50, 5, 1);
    */
}

void test_machine_creation_and_deletion(void)
{
    // Arrange
    Graph *startGraph = create_graph(3,1);
    RestrictionParameters *params = initialize_restriction_parameters();
    params->numIntParams = 1;
    params->intParams = gtd_malloc(sizeof(int));
    params->intParams[0] = 10;


    // Act
    GenerativeProofMachine *machine = create_proof_machine(startGraph);
    params->machine = machine;
    add_restriction(machine, create_restriction(MaxVertexDegreeFact, params));
    destroy_generative_proof_machine(machine);

    // Assert
}

void test_max_degree_restriction(void)
{
    // Arrange
    Graph *startGraph = create_graph(10,10);
    set_edge_connected(startGraph, 0, 1);
    set_edge_connected(startGraph, 0, 2);
    set_edge_connected(startGraph, 0, 3);
    GenerativeProofMachine *machine = create_proof_machine(startGraph);
    char **adjMatrix = get_graph_adjacency_matrix(startGraph);
    RestrictionParameters *params = initialize_restriction_parameters();
    params->numIntParams = 1;
    params->intParams = (int *)gtd_malloc(sizeof(int));
    params->intParams[0] = 3;
    params->machine = machine;
    add_restriction(machine, create_restriction(MaxVertexDegreeFact, params));
    // add_restriction(machine, create_max_degree_restriction(3, machine);

    // Act
    execute_generative_proof_machine(machine);

    // Assert
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

    // Arrange
    set_edge_connected(startGraph, 0, 4);

    // Act
    int contr = execute_generative_proof_machine(machine);

    // Assert
    assert(contr == 1);

    // Cleanup
    destroy_generative_proof_machine(machine);
}

void test_no_k_cycle_restriction(void)
{
    // ====== test 1 ======
    // Arrange
    Graph *graph1 = create_graph(4, 4);
    set_edge_connected(graph1, 0, 1);
    set_edge_connected(graph1, 1, 2);
    set_edge_connected(graph1, 2, 3);

    // Act
    PathNode *paths = find_k_paths(graph1, 4);
    int num_paths = 0;
    while(paths != NULL)
    {
        num_paths++;
        paths = paths->next;
    }

    // Assert
    assert(num_paths == 2);

    // Arrange
    GenerativeProofMachine *machine = create_proof_machine(graph1);
    RestrictionParameters *params1 = initialize_restriction_parameters();
    params1->numIntParams = 1;
    params1->intParams = (int *)gtd_malloc(sizeof(int));
    params1->intParams[0] = 4;
    params1->machine = machine;
    // add_restriction(machine, create_no_k_cycle_restriction(4, machine);
    add_restriction(machine, create_restriction(HasNoCycleFact, params1));

    // Act
    int contr1 = execute_generative_proof_machine(machine);

    // Assert
    assert(contr1 == 0);
    assert(get_graph_adjacency_matrix(graph1)[0][3] == NOT_CONNECTED_SYMBOL);

    // Arrange
    set_edge_connected(graph1, 0, 3);

    // Act
    int contr2 = execute_generative_proof_machine(machine);

    // Assert
    assert(contr2 == 1);

    // ====== test 2 ======

    // Arrange
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

    // Act
    paths = find_k_paths(graph2, 2);
    num_paths = 0;
    while(paths != NULL)
    {
        num_paths++;
        paths = paths->next;
    }

    // Assert
    assert(num_paths == 28);

    // ===== test 3 =====

    // Arrange
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

    // Act
    machine = create_proof_machine(graph3);
    RestrictionParameters *params3 = initialize_restriction_parameters();
    params3->numIntParams = 1;
    params3->intParams = (int *)gtd_malloc(sizeof(int));
    params3->intParams[0] = 5;
    params3->machine = machine;
    add_restriction(machine, create_restriction(HasNoCycleFact, params3));
    int contr3 = execute_generative_proof_machine(machine);

    // Assert
    assert(contr3 == 1);

    // ===== test 4 =====

    // Arrange
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

    // Act
    machine = create_proof_machine(graph4);
    // add_restriction(machine, create_no_k_cycle_restriction(5, machine);
    RestrictionParameters *params4 = initialize_restriction_parameters();
    params4->numIntParams = 1;
    params4->intParams = (int *)gtd_malloc(sizeof(int));
    params4->intParams[0] = 5;
    params4->machine = machine;
    add_restriction(machine, create_restriction(HasNoCycleFact, params4));
    int contr4 = execute_generative_proof_machine(machine);

    // Assert
    assert(contr4 == 0);

}

void test_no_induced_path_k_restriction(void)
{
    // ===== test 1 =====
    // Arrange
    Graph *startGraph = create_graph(10, 10);
    char **adjMatrix = get_graph_adjacency_matrix(startGraph);
    set_edge_connected(startGraph, 0, 1);
    set_edge_connected(startGraph, 1, 2);
    GenerativeProofMachine *machine = create_proof_machine(startGraph);
    RestrictionParameters *params1 = initialize_restriction_parameters();
    params1->numIntParams = 1;
    params1->intParams = (int *)gtd_malloc(sizeof(int));
    params1->intParams[0] = 3;
    params1->machine = machine;
    // add_restriction(machine, create_no_induced_path_k_restriction(3, machine);
    add_restriction(machine, create_restriction(HasNoInducedPathFact, params1));

    // Act
    int contr1 = execute_generative_proof_machine(machine);

    // Assert
    assert(contr1 == 0);
    assert(adjMatrix[0][1] == CONNECTED_SYMBOL);
    assert(adjMatrix[1][2] == CONNECTED_SYMBOL);
    assert(adjMatrix[0][2] == CONNECTED_SYMBOL);

    // ===== test 2 =====

    // Arrange
    Graph *graph2 = create_graph(5, 5);
    adjMatrix = get_graph_adjacency_matrix(graph2);
    machine = create_proof_machine(graph2);

    RestrictionParameters *params2 = initialize_restriction_parameters();
    params2->numIntParams = 1;
    params2->intParams = (int *)gtd_malloc(sizeof(int));
    params2->intParams[0] = 5;
    params2->machine = machine;
    add_restriction(machine, create_restriction(HasNoInducedPathFact, params2));
    // add_restriction(machine, create_no_induced_path_k_restriction(5, machine);
    set_edge_connected(graph2, 0, 1);
    set_edge_connected(graph2, 1, 2);
    set_edge_connected(graph2, 2, 3);
    set_edge_connected(graph2, 3, 4);

    // Act
    int contr2 = execute_generative_proof_machine(machine);

    // Assert
    assert(contr2 == 0);

    // Arrange
    set_edge_not_connected(graph2, 0, 2);
    set_edge_not_connected(graph2, 0, 3);
    set_edge_not_connected(graph2, 0, 4);
    set_edge_not_connected(graph2, 1, 3);
    set_edge_not_connected(graph2, 1, 4);
    
    // Act
    contr2 = execute_generative_proof_machine(machine);

    // Assert
    assert(contr2 == 0);
    assert(adjMatrix[2][4] == CONNECTED_SYMBOL);

    // ===== test 3 =====

    // Arrange
    Graph *graph3 = create_graph(6, 6);
    GenerativeProofMachine *machine3 = create_proof_machine(graph3);
    RestrictionParameters *params3 = initialize_restriction_parameters();
    params3->numIntParams = 1;
    params3->intParams = (int *)gtd_malloc(sizeof(int));
    params3->intParams[0] = 2;
    params3->machine = machine3;
    add_restriction(machine3, create_restriction(HasNoInducedPathFact, params3));
    // get_machine_restrictions(machine3)[0] = create_no_induced_path_k_restriction(2, machine);
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
    GenerativeProofMachine *machine4 = create_proof_machine(graph4);
    RestrictionParameters *params4 = initialize_restriction_parameters();
    params4->numIntParams = 1;
    params4->intParams = (int *)gtd_malloc(sizeof(int));
    params4->intParams[0] = 2;
    params4->machine = machine;
    add_restriction(machine4, create_restriction(HasNoInducedPathFact, params4));
    // get_machine_restrictions(machine4)[0] = create_no_induced_path_k_restriction(2, machine4);
    int contr4;

    // Act
    contr4 = execute_generative_proof_machine(machine4);

    // Assert
    assert(contr4 == 1);

    // (⊙_☉)
}


void test_erdos_gyarfas_case(void)
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
    GenerativeProofMachine *machine1 = create_proof_machine(graph1);
    RestrictionParameters *params1 = initialize_restriction_parameters();
    params1->numIntParams = 1;
    params1->intParams = (int *)gtd_malloc(sizeof(int));
    params1->intParams[0] = 4;
    params1->machine = machine1;
    add_restriction(machine1,  create_restriction(HasNoCycleFact, params1));
    // add_restriction(machine1,  create_no_k_cycle_restriction(4, machine1);
    RestrictionParameters *params2 = initialize_restriction_parameters();
    params2->numIntParams = 1;
    params2->intParams = (int *)gtd_malloc(sizeof(int));
    params2->intParams[0] = 8;
    params2->machine = machine1;
    add_restriction(machine1, create_restriction(HasNoCycleFact, params2));
    // get_machine_restrictions(machine1)[1] = create_no_k_cycle_restriction(8, machine1);
    RestrictionParameters *params3 = initialize_restriction_parameters();
    params3->numIntParams = 1;
    params3->intParams = (int *)gtd_malloc(sizeof(int));
    params3->intParams[0] = 8;
    params3->machine = machine1;
    add_restriction(machine1, create_restriction(HasNoInducedPathFact, params3));
    // get_machine_restrictions(machine1)[2] = create_no_induced_path_k_restriction(8, machine1);
    RestrictionParameters *params4 = initialize_restriction_parameters();
    params4->numIntParams = 1;
    params4->intParams = (int *)gtd_malloc(sizeof(int));
    params4->intParams[0] = 3;
    params4->machine = machine1;
    add_restriction(machine1, create_restriction(MinVertexDegreeFact, params4));
    // get_machine_restrictions(machine1)[3] = create_min_degree_restriction(3, machine1);
    RestrictionParameters *params5 = initialize_restriction_parameters();
    params5->numIntParams = 1;
    params5->intParams = (int *)gtd_malloc(sizeof(int));
    params5->intParams[0] = 3;
    params5->machine = machine1;
    add_restriction(machine1, create_restriction(HasNoUnknownEdgesFact, params5));
    // get_machine_restrictions(machine1)[4] = create_edge_check_restriction(3, machine1);

    // Act
    int contr1 = execute_generative_proof_machine(machine1);
    write_proof_tree(get_machine_proof_tree(machine1), stdout);

    // Assert
    assert(contr1 == 1);
}

void test_minimum_degree_restriction(void)
{
    // ========== test 1 ==========

    // Arrange
    Graph *graph1 = create_graph(10, 1);
    GenerativeProofMachine *machine1 = create_proof_machine(graph1);
    RestrictionParameters *params1 = initialize_restriction_parameters();
    params1->numIntParams = 1;
    params1->intParams = (int *)gtd_malloc(sizeof(int));
    params1->intParams[0] = 7;
    params1->machine = machine1;
    add_restriction(machine1,  create_restriction(MinVertexDegreeFact, params1));
    // add_restriction(machine1,  create_min_degree_restriction(7, machine1);
    int *degree1 = get_graph_degree(graph1);
    GTD_UNUSED(degree1);

    // Act
    int contr1 = execute_generative_proof_machine(machine1);

    // Assert
    assert(contr1 == 0);
    // assert(degree1[0] == 7);

    // ========== test 2 ==========

    // Arrange
    Graph *graph2 = create_graph(10, 3);
    GenerativeProofMachine *machine2 = create_proof_machine(graph2);
    RestrictionParameters *params2 = initialize_restriction_parameters();
    params2->numIntParams = 1;
    params2->intParams = (int *)gtd_malloc(sizeof(int));
    params2->intParams[0] = 15;
    params2->machine = machine2;
    add_restriction(machine2, create_restriction(MinVertexDegreeFact, params2));
    // get_machine_restrictions(machine2)[0] = create_min_degree_restriction(15, machine2);
    int *degree2 = get_graph_degree(graph2);
    GTD_UNUSED(degree2);

    // Act
    int contr2 = execute_generative_proof_machine(machine2);

    // Assert
    assert(contr2 == 0);
    // assert(degree2[0] == 7);
}

void test_erdos_gyarfas_pk_free(int k, int max_vertices, int max_depth, int save_to_file)
{
    GTD_UNUSED(save_to_file);

    for(int t = k; t >= 5; --t)
    {
        Graph *graph = create_graph(max_vertices, 0);
        GenerativeProofMachine *machine = create_proof_machine(graph);

        RestrictionParameters *params0 = initialize_restriction_parameters();
        params0->numIntParams = 1;
        params0->intParams = (int *)gtd_malloc(sizeof(int));
        params0->intParams[0] = t;
        params0->machine = machine;
        add_restriction(machine, create_restriction(HasInducedCycleFact, params0));

        RestrictionParameters *params1 = initialize_restriction_parameters();
        params1->numIntParams = 1;
        params1->intParams = (int *)gtd_malloc(sizeof(int));
        params1->intParams[0] = 4;
        params1->machine = machine;
        add_restriction(machine, create_restriction(HasNoCycleFact, params1));
        
        RestrictionParameters *params2 = initialize_restriction_parameters();
        params2->numIntParams = 1;
        params2->intParams = (int *)gtd_malloc(sizeof(int));
        params2->intParams[0] = 8;
        params2->machine = machine;
        add_restriction(machine, create_restriction(HasNoCycleFact, params2));

        RestrictionParameters *params3 = initialize_restriction_parameters();
        params3->numIntParams = 1;
        params3->intParams = (int *)gtd_malloc(sizeof(int));
        params3->intParams[0] = k;
        params3->machine = machine;
        add_restriction(machine, create_restriction(HasNoInducedPathFact, params3));

        RestrictionParameters *params4 = initialize_restriction_parameters();
        params4->numIntParams = 1;
        params4->intParams = (int *)gtd_malloc(sizeof(int));
        params4->intParams[0] = 3;
        params4->machine = machine;
        add_restriction(machine, create_restriction(MinVertexDegreeFact, params4));
        
        RestrictionParameters *params5 = initialize_restriction_parameters();
        params5->numIntParams = 1;
        params5->intParams = (int *)gtd_malloc(sizeof(int));
        params5->intParams[0] = max_depth;
        params5->machine = machine;
        add_restriction(machine, create_restriction(HasNoUnknownEdgesFact, params5));

        printf("Proving for t = %d... ", t);
        fflush(stdout);
        int contr = execute_generative_proof_machine(machine);
        printf("Done!\n");
        assert(contr == 1);

        if(save_to_file)
        {   
            struct stat sb;
            if((stat("out-data", &sb) == 0 && S_ISDIR(sb.st_mode)) == 0)
            {
                mkdir("out-data", 0777);
            }

            char filename[100];
            sprintf(filename, "out-data/p%d_free_with_c%d.txt", k, t);
            FILE *file = fopen(filename, "w");
            write_proof_tree(get_machine_proof_tree(machine), file);
            fclose(file);
        }
    }
}