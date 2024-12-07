#ifndef GENERATIVE_PROOF_MACHINE_H
#define GENERATIVE_PROOF_MACHINE_H

#include "common.h"
#include "physical_graph.h"
#include "proof_tree.h"


typedef struct GraphNode GraphNode;
Graph *get_node_graph(GraphNode *node);
GraphNode *get_node_next(GraphNode *node);

typedef struct GenerativeRestriction GenerativeRestriction;
typedef struct GenerativeProofMachine GenerativeProofMachine;
GenerativeProofMachine *create_proof_machine(int num_restrictions, Graph *startGraph);
int destroy_generative_proof_machine(GenerativeProofMachine *machine);
uint8_t execute_generative_proof_machine(GenerativeProofMachine *machine);
void set_machine_depth(GenerativeProofMachine *machine, int depth);
int get_machine_depth(GenerativeProofMachine *machine);
Graph *get_machine_graph(GenerativeProofMachine *machine);
GenerativeProofMachine *copy_proof_machine(GenerativeProofMachine *machine);
ProofTree *get_machine_proof_tree(GenerativeProofMachine *machine);
GenerativeRestriction **get_machine_restrictions(GenerativeProofMachine *machine);

#endif
