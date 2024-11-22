#include "generative_proof_machine.h"
#include "physical_graph.h"
#include "generative_restriction.h"
#include "common.h"


struct GenerativeProofMachine
{
    Graph *graph;
    GenerativeRestriction **restrictions;
    int num_restrictions;
};


GenerativeProofMachine *create_generative_proof_machine(GenerativeRestriction **restrictions,
 int num_restrictions, Graph* startGraph)
{
    GenerativeProofMachine *generativeProofMachine = gtd_malloc(sizeof(GenerativeProofMachine));
    generativeProofMachine->restrictions = restrictions;
    generativeProofMachine->num_restrictions = num_restrictions;
    generativeProofMachine->graph = startGraph;

    return generativeProofMachine;
}

int destroy_generative_proof_machine(GenerativeProofMachine *machine)
{
    destroy_graph(machine->graph);
    gtd_free(machine);

    return 1;
}



// returns all graphs meeting all restrictions
int execute_generative_proof_machine(GenerativeProofMachine *machine)
{
    int i = 0;
    int num_restrictions = machine->num_restrictions;
    while(i < num_restrictions)
    {
        GenerativeRestriction *restriction = machine->restrictions[i];
        Graph *graph = machine->graph;
        RestrictionResult *result = check_restriction(graph, restriction);
        if(result->contradictionFound)
        {
            // todo memory clear
            return 1;
        }
        if(result->modified)
        {
            // todo memory clear
            i = 0;
            continue;
        }
        ++i;
    }
    return 0;
}
