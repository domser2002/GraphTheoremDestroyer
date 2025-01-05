#ifndef FACT_TREE_MACHINE_H
#define FACT_TREE_MACHINE_H
#ifdef IS_FACT_TREE_COMPONENT
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
void write_deduction(FactTree *FactTree, uint32_t idx, FILE *output);
// getters
bool get_contradiction_found(FactTreeMachine *machine);
FactTree *get_fact_tree(FactTreeMachine *machine);
Fact **get_contradicting_facts(FactTreeMachine *machine, uint32_t *contradicting_count);
#endif
#endif