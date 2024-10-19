#ifndef MACHINE_H
#define MACHINE_H
#include "graph.h"
#include "common.h"
#include "fact.h"
#include "contradiction.h"
#include "implication.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct ProofMachine ProofMachine;

ProofMachine *init_machine(FactTree *FactTree);
void delete_machine(ProofMachine *machine);
void execute(ProofMachine *machine);
bool write_proof(ProofMachine *machine, FILE *output);

#endif