#ifndef FACT_TREE_MACHINE_H
#define FACT_TREE_MACHINE_H
#include "fact_tree.h"
#include "common.h"
#include "fact.h"
#include "contradiction.h"
#include "implication.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct FactTreeMachine FactTreeMachine;

FactTreeMachine *init_machine(FactTree *FactTree);
void delete_machine(FactTreeMachine *machine);
void execute(FactTreeMachine *machine);
bool write_proof(FactTreeMachine *machine, FILE *output);

#endif