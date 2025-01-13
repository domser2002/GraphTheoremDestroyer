#ifndef GENERATIVE_PROOF_MACHINE_H
#define GENERATIVE_PROOF_MACHINE_H
#ifdef IS_GENERATIVE_MODULE_COMPONENT
#include "common.h"
#include "physical_graph.h"
#include "proof_tree.h"


typedef struct GraphNode GraphNode;
Graph *get_node_graph(GraphNode *node);
GraphNode *get_node_next(GraphNode *node);

typedef struct GenerativeRestriction GenerativeRestriction;
typedef struct GenerativeProofMachine GenerativeProofMachine;
/**
 * \brief constructor for GenerativeProofMachine class
 * \param startGraph start graph
 * \returns pointer to a newly created GenerativeProofMachine
 */
GenerativeProofMachine *create_proof_machine(Graph *startGraph);

/**
 * \brief function to destruct GenerativeProofMachine
 * \param machine pointer to the GenerativeProofMachine to destroy
 * \returns 1 if suceeded, otherwise raises fault
 */
int destroy_generative_proof_machine(GenerativeProofMachine *machine);

/**
 * \brief function for executing generative proof machine
 * \param machine pointer to the GenerativeProofMachine to execute
 * \returns 1 if contradictionw was found, 0 otherwise
 */
uint8_t execute_generative_proof_machine(GenerativeProofMachine *machine);

/**
 * \brief function to set depth parameter of GenerativeProofMachine class
 * \param machine pointer to the GenerativeProofMachine, which depth will be set
 * \param depth new depth value
 */
void set_machine_depth(GenerativeProofMachine *machine, int depth);

/**
 * \brief function to get machine depth
 * \param machine pointer to the GenerativeProofMachine for which depth will be returned
 * \returns depth of a machine
 */
int get_machine_depth(GenerativeProofMachine *machine);

/**
 * \brief function to get machine graph
 * \param machine pointer to the GenerativeProofMachine for which start graph will be returned
 * \returns start graph of a machine
 */
Graph *get_machine_graph(GenerativeProofMachine *machine);

/**
 * \brief function to make a deep copy of a machine, except for the proof tree of the machine
 * \param machine pointer to the GenerativeProofMachine, which will be copied
 * \returns returns a deep copy of a machine, but proofTree is initialized to new, clean instance
 */
GenerativeProofMachine *copy_proof_machine(GenerativeProofMachine *machine);

/**
 * \brief function to get proof tree of a graph
 * \param machine pointer to the GenerativeProofMachine for which proof tree will be returned
 * \return proof tree of a machine
 */
ProofTree *get_machine_proof_tree(GenerativeProofMachine *machine);

/**
 * \brief function to get restrictions of a graph
 * \param machine pointer to the GenerativeProofMachine for which restrictions tree will be returned
 * \return restrictions of a machine
 */
GenerativeRestriction **get_machine_restrictions(GenerativeProofMachine *machine);

/**
 * \brief function to load everything from GenerativeProofMachine m2(except proof tree) into m1
 * \brief Warning: the copy will be shallow!
 * \param m1 GenerativeProofMachine that data will be loaded into
 * \param m2 GenerativeProofMachine from data will be loaded from
 */
void load_machine(GenerativeProofMachine *m1, GenerativeProofMachine *m2);

/**
 * \brief function to add new restriction to the generative proof machine
 * \param machine GenerativeProofMachine that new restriction will be added to
 * \param restriction GenerativeRestriction that will be added at the end of the restriction array in machine
 */
void add_restriction(GenerativeProofMachine *machine, GenerativeRestriction *restriction);

void load_machine(GenerativeProofMachine *m1, GenerativeProofMachine *m2);
#endif
#endif