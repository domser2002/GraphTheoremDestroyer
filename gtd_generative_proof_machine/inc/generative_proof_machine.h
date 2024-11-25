#ifndef GENERATIVE_PROOF_MACHINE_H
#define GENERATIVE_PROOF_MACHINE_H

#include "common.h"
#include "physical_graph.h"

// Forward declaration of GenerativeRestriction
typedef struct GenerativeRestriction GenerativeRestriction;

typedef struct GraphNode GraphNode;
typedef struct GenerativeProofMachine GenerativeProofMachine;

Graph *get_node_graph(GraphNode *node);
GraphNode *get_node_next(GraphNode *node);

GenerativeProofMachine *create_generative_proof_machine(GenerativeRestriction **restrictions,
 int num_restrictions, Graph* startGraph);

int destroy_generative_proof_machine(GenerativeProofMachine *machine);
int execute_generative_proof_machine(GenerativeProofMachine *machine);
void set_machine_depth(GenerativeProofMachine *machine, int depth);
int get_machine_depth(GenerativeProofMachine *machine);
Graph *get_machine_graph(GenerativeProofMachine *machine);
GenerativeProofMachine *copyMachine(GenerativeProofMachine *machine);

#endif
