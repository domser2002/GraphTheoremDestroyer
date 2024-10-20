#ifndef GENERATIVE_PROOF_MACHINE_H
#define GENERATIVE_PROOF_MACHINE_H
#include "common.h"
#include "physical_graph.h"
#include "generative_restriction.h"

typedef struct GraphNode GraphNode;
typedef struct GenerativeProofMachine GenerativeProofMachine;

GenerativeProofMachine *create_generative_proof_machine(GenerativeRestriction **restrictions, int num_restrictions);
int destroy_generative_proof_machine(GenerativeProofMachine *machine);

void add_graph(GenerativeProofMachine *machine, Graph *graph);
void clear_graphs(GenerativeProofMachine *machine, int delete);

int execute_generative_proof_machine(GenerativeProofMachine *machine);

int int_pow(int base, int exp);

#endif