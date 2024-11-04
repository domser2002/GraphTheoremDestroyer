#ifndef FACT_TREE_H
#define FACT_TREE_H
#include <stdint.h>
#include <stdbool.h>
#include "fact.h"

typedef struct FactTree
{
    uint32_t fact_count;
    uint32_t **parents; 
    uint8_t *parent_count;
    Fact **facts;
} FactTree;

FactTree *construct(uint32_t fact_count, Fact **facts);
void destruct(FactTree* g);
bool add_fact(FactTree *g, uint32_t *parent_idxs, uint8_t parent_count, Fact *new_fact);
#endif