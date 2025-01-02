#ifndef IMPLICATION_H
#define IMPLICATION_H
#ifdef IS_FACT_TREE_COMPONENT
#define MAX_LEFT_SIDE_FACTS 2
#define MIN_LEFT_SIDE_FACTS 1
#include "fact.h"
#include <string.h>
Fact **implies(Fact **factArray, uint32_t n_facts, int *count);
#endif
#endif