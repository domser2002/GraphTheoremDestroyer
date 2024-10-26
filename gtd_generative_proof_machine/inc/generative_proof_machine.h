#ifndef GENERATIVE_PROOF_MACHINE_H
#define GENERATIVE_PROOF_MACHINE_H
#include "common.h"
#include "physical_graph.h"
#include "generative_restriction.h"

typedef struct GraphNode GraphNode;
typedef struct GenerativeProofMachine GenerativeProofMachine;

GenerativeProofMachine *create_generative_proof_machine(GenerativeRestriction **restrictions,
 int num_restrictions, int num_hash_buckets);
int destroy_generative_proof_machine(GenerativeProofMachine *machine);

void add_graph(GenerativeProofMachine *machine, Graph *graph);
void clear_graphs(GenerativeProofMachine *machine, int delete);

// check if graph passes all the machine restrictions and is not isomorphic
// to any graph in machine
int can_add_graph(GenerativeProofMachine *machine, Graph *graph);

int execute_generative_proof_machine(GenerativeProofMachine *machine);

int generate_graphs_from_bucket(
    GenerativeProofMachine *machine,
    GraphNode *bucket);

#endif