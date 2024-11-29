#include "generative_proof_machine.h"
#include "physical_graph.h"
#include "generative_restriction.h"
#include "common.h"
// #include "proof_tree.h"


struct GenerativeProofMachine
{
    Graph *graph;
    GenerativeRestriction **restrictions;
    int num_restrictions;
    int depth;
    ProofTree *proofTree;
};


GenerativeProofMachine *create_generative_proof_machine(GenerativeRestriction **restrictions,
 int num_restrictions, Graph* startGraph)
{
    GenerativeProofMachine *generativeProofMachine = gtd_malloc(sizeof(GenerativeProofMachine));
    generativeProofMachine->restrictions = restrictions;
    generativeProofMachine->num_restrictions = num_restrictions;
    generativeProofMachine->graph = startGraph;
    generativeProofMachine->depth = 0;
    generativeProofMachine->proofTree = initProofTree();

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


void set_machine_depth(GenerativeProofMachine *machine, int depth)
{
    machine->depth = depth;
}

int get_machine_depth(GenerativeProofMachine *machine)
{
    int depth = machine->depth;
    return depth;
}


Graph *get_machine_graph(GenerativeProofMachine *machine)
{
    return machine->graph;
}

// copies everything excpept of the proof tree
GenerativeProofMachine *copyMachine(GenerativeProofMachine *machine)
{
    //GenerativeRestriction **restrictions = machine->restrictions;
    int num_restrictions = machine->num_restrictions;
    GenerativeRestriction **restrictions = gtd_malloc(sizeof(GenerativeRestriction*) * num_restrictions);
    Graph *startGraph = copyGraph(machine->graph);
    int depth = machine->depth;

    GenerativeProofMachine *result = create_generative_proof_machine(restrictions, num_restrictions, startGraph);
    for(int i = 0; i < num_restrictions; ++i)
    {
        GenerativeRestriction *restr = copy_restriction(machine->restrictions[i]);
        RestrictionParameters *params = get_restriction_parameters(restr);
        params->machine = result;
        restrictions[i] = restr;
    }
    result->depth = depth;

    result->proofTree = initProofTree();

    return result;
}


ProofTree *get_machine_proof_tree(GenerativeProofMachine *machine)
{
    return machine->proofTree;
}