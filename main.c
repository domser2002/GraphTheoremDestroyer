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
    Function *params_2[3];
    params_2[0] = create(2);
    params_2[1] = create(1);
    params_2[2] = create(3);
    Function *params_3[1];
    params_3[0] = create(4);
    Function *params_4[1];
    params_4[0] = create(8);
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