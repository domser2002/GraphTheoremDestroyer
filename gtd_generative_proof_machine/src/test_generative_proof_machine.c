#include "test_generative_proof_machine.h"
// #include "physical_graph.h"
#include "generative_restriction.h"
#include "generative_proof_machine.h"
#include <assert.h>

void test_generative_proof_machine(void)
{
    printf("Creation and deletion\n");
    creation_and_deletion();
    printf("Small machine\n");
    execute_small_machine();
}

void creation_and_deletion(void)
{
    GenerativeRestriction *restrictions = create_max_vertices_restriction(10);
    GenerativeProofMachine *machine = create_generative_proof_machine(&restrictions, 1, 10);

    destroy_generative_proof_machine(machine);
}

void execute_small_machine(void)
{
    GenerativeRestriction *restrictions = create_max_vertices_restriction(7);
    GenerativeProofMachine *machine = create_generative_proof_machine(&restrictions, 1, 50);

    Graph *graph = create_graph(11, 1);
    add_graph(machine, graph);

    execute_generative_proof_machine(machine);

    destroy_generative_proof_machine(machine);
}