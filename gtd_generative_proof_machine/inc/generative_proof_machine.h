#ifndef GENERATIVE_PROOF_MACHINE_H
#define GENERATIVE_PROOF_MACHINE_H
#include "common.h"
#include "physical_graph.h"
#include "generative_restriction.h"
#include "common.h"

typedef struct GraphNode GraphNode;
typedef struct GenerativeProofMachine GenerativeProofMachine;

Graph *get_node_graph(GraphNode *node);

GraphNode *get_node_next(GraphNode *node);

GenerativeProofMachine *create_generative_proof_machine(GenerativeRestriction **restrictions,
 int num_restrictions, Graph* startGraph);

int destroy_generative_proof_machine(GenerativeProofMachine *machine);

int execute_generative_proof_machine(GenerativeProofMachine *machine);

#endif