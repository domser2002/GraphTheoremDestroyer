#define IS_GENERATIVE_MODULE_COMPONENT
#include "generative_proof_machine.h"
#include "physical_graph.h"
#include "generative_restriction.h"
#include "common.h"

typedef struct GenerativeProofMachine
{
    Graph *graph;
    GenerativeRestriction **restrictions;
    int numRestrictions;
    int depth;
    ProofTree *proofTree;
} GenerativeProofMachine;


GenerativeProofMachine *create_proof_machine(Graph *startGraph)
{
    GenerativeProofMachine *machine = gtd_malloc(sizeof(GenerativeProofMachine));
    machine->restrictions = NULL;
    machine->numRestrictions = 0;
    machine->graph = startGraph;
    machine->depth = 0;
    machine->proofTree = create_proof_tree();

    return machine;
}

int destroy_generative_proof_machine(GenerativeProofMachine *machine)
{
    destroy_graph(machine->graph);
    for(int i=0;i<machine->numRestrictions;i++)
    {
        delete_restriction_object(machine->restrictions[i]);
    }
    delete_proof_tree(machine->proofTree);
    gtd_free(machine);
    return 1;
}

uint8_t execute_generative_proof_machine(GenerativeProofMachine *machine)
{
    int i = 0;
    while (i < machine->numRestrictions)
    {
        GenerativeRestriction *restriction = machine->restrictions[i];
        if(get_restriction_block(restriction) == -1)
        {
            ++i;
            continue;
        }
        if(get_restriction_block(restriction) == 1)
        {
            set_restriction_block(restriction, -1);
        }
        Graph *graph = machine->graph;
        RestrictionResult *result = validate_restriction(graph, restriction);

        if (result->contradictionFound)
        {
            // TODO: Implement proper memory cleanup
            return 1;
        }

        if (result->modified)
        {
            // TODO: Implement proper memory cleanup
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
    return machine->depth;
}

Graph *get_machine_graph(GenerativeProofMachine *machine)
{
    return machine->graph;
}

/**
 * \brief function to make a deep copy of a machine, except for the proof tree of the machine
 * \param machine pointer to the GenerativeProofMachine, which will be copied
 * \returns returns a deep copy of a machine, but proofTree is initialized to new, clean instance
 */
GenerativeProofMachine *copy_proof_machine(GenerativeProofMachine *machine)
{
    int numRestrictions = machine->numRestrictions;
    Graph *graphCopy = copy_graph(machine->graph);
    int depth = machine->depth;

    GenerativeProofMachine *copy = create_proof_machine(graphCopy);

    for (int i = 0; i < numRestrictions; ++i)
    {
        GenerativeRestriction *restrictionCopy = deep_copy_restriction(machine->restrictions[i]);
        RestrictionParameters *params = get_parameters_from_restriction(restrictionCopy);
        params->machine = copy;
        add_restriction(copy,restrictionCopy);
    }

    copy->depth = depth;
    copy->proofTree = create_proof_tree();

    return copy;
}

/**
 * \brief function to get proof tree of a graph
 * \param machine pointer to the GenerativeProofMachine for which proof tree will be returned
 * \return proof tree of a machine
 */
ProofTree *get_machine_proof_tree(GenerativeProofMachine *machine)
{
    return machine->proofTree;
}

GenerativeRestriction **get_machine_restrictions(GenerativeProofMachine *machine)
{
    return machine->restrictions;
}

void load_machine(GenerativeProofMachine *m1, GenerativeProofMachine *m2)
{
    destroy_graph(m1->graph);
    m1->graph = m2->graph;
    GenerativeRestriction **restrictions = get_machine_restrictions(m1);
    for(int i = 0; i < m1->numRestrictions; ++i)
    {
        destroy_restriction_parameters_soft(get_parameters_from_restriction(restrictions[i]));
        delete_restriction_object(restrictions[i]);
    }
    m1->depth = m2->depth;
    m1->restrictions = m2->restrictions;
    m1->numRestrictions = m2->numRestrictions;
    gtd_free(restrictions);
}

void add_restriction(GenerativeProofMachine *machine, GenerativeRestriction *restriction)
{
    if(restriction == NULL)
        return;
    machine->numRestrictions++;
    machine->restrictions = (GenerativeRestriction**)gtd_realloc(machine->restrictions, machine->numRestrictions * sizeof(GenerativeRestriction*));
    machine->restrictions[machine->numRestrictions-1] = restriction;
    return;
}