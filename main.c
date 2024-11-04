#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "fact_tree.h"
#include "fact.h"
#include "fact_tree_machine.h"

Fact **generate_facts(int *count)
{
    *count = 2;
    Fact **factArray = gtd_malloc(*count * sizeof(Fact*));
    int min_edge_count[] = {11};
    int max_vertex_count[] = {5};
    factArray[0] = create_fact(MinEdgeCountFact,min_edge_count,1);
    factArray[1] = create_fact(MaxVertexCountFact,max_vertex_count,1);
    return factArray;
}

int main(void)
{
    GTD_LOG("GraphTheoremDestroyer initiated. Starting application");
    int count = 0;
    Fact **factArray = generate_facts(&count);
    FactTree *FactTree = construct(count,factArray);
    FactTreeMachine *machine = init_machine(FactTree);
    execute(machine);
    write_proof(machine,stdout);
    delete_machine(machine);
    destruct(FactTree);
    GTD_LOG("Terminating application");
    return EXIT_SUCCESS;
}