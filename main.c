#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "fact_tree.h"
#include "fact.h"
#include "fact_tree_machine.h"

Fact **generate_facts(int *count)
{
    *count = 5;
    Fact **factArray = gtd_malloc(*count * sizeof(Fact*));
    int params_2[] = {2,1,3};
    int params_3[] = {4};
    int params_4[] = {8};
    factArray[0] = create_fact(HasNoCyclesFact,NULL,0);
    factArray[1] = create_fact(HasNoInducedCompletePartiteFact,params_2,3);
    factArray[2] = create_fact(HasNoInducedPathFact,params_3,1);
    factArray[3] = create_fact(MinVertexCountFact,params_4,1);
    factArray[4] = create_fact(IsConnectedFact,NULL,0);
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