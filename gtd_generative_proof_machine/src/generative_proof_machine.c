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

/**
 * \brief constructor for GenerativeProofMachine class
 * \param startGraph start graph
 * \returns pointer to a newly created GenerativeProofMachine
 */
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

/**
 * \brief function to destruct GenerativeProofMachine
 * \param machine pointer to the GenerativeProofMachine to destroy
 * \returns 1 if suceeded, otherwise raises fault
 */
int destroy_generative_proof_machine(GenerativeProofMachine *machine)
{
    destroy_graph(machine->graph);
    gtd_free(machine);

    return 1;
}

/**
 * \brief function for executing generative proof machine
 * \param machine pointer to the GenerativeProofMachine to execute
 * \returns 1 if contradictionw was found, 0 otherwise
 */
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

/**
 * \brief function to set depth parameter of GenerativeProofMachine class
 * \param machine pointer to the GenerativeProofMachine, which depth will be set
 * \param depth new depth value
 */
void set_machine_depth(GenerativeProofMachine *machine, int depth)
{
    machine->depth = depth;
}

/**
 * \brief function to get machine depth
 * \param machine pointer to the GenerativeProofMachine for which depth will be returned
 * \returns depth of a machine
 */
int get_machine_depth(GenerativeProofMachine *machine)
{
    return machine->depth;
}

/**
 * \brief function to get machine graph
 * \param machine pointer to the GenerativeProofMachine for which start graph will be returned
 * \returns start graph of a machine
 */
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

/**
 * \brief function to get restrictions of a graph
 * \param machine pointer to the GenerativeProofMachine for which restrictions tree will be returned
 * \return restrictions of a machine
 */
GenerativeRestriction **get_machine_restrictions(GenerativeProofMachine *machine)
{
    return machine->restrictions;
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

/**
 * \brief function to load everything from GenerativeProofMachine m2(except proof tree) into m1
 * \brief Warning: the copy will be shallow!
 * \param m1 GenerativeProofMachine that data will be loaded into
 * \param m2 GenerativeProofMachine from data will be loaded from
 */
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