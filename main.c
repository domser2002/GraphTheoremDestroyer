#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "graph.h"
#include "fact.h"
#include "machine.h"

Fact **generate_facts(int *count)
{
    *count = 2;
    Fact **factArray = gtd_malloc(*count * sizeof(Fact*));
    factArray[0] = create_min_edge_count_fact(11);
    factArray[1] = create_max_vertex_count_fact(5);
    return factArray;
}

void free_facts(Fact **factArray)
{
    delete_max_vertex_count_fact(factArray[1]);
    delete_min_edge_count_fact(factArray[0]);
    gtd_free(factArray);
}

int main(void)
{
    GTD_LOG("GraphTheoremDestroyer initiated. Starting application");
    int count = 0;
    Fact **factArray = generate_facts(&count);
    FactTree *FactTree = construct(count);
    FactTree->vertexData = (void**)factArray;
    ProofMachine *machine = init_machine(FactTree);
    execute(machine);
    write_proof(machine,stdout);
    delete_machine(machine);
    destruct(FactTree);
    free_facts(factArray);
    GTD_LOG("Terminating application");
    return EXIT_SUCCESS;
}