#ifndef CONTRADICTION_H
#define CONTRADICTION_H
#ifdef IS_FACT_TREE_COMPONENT
#include "fact.h"
#include "function.h"
#define MIN_CONTRADICTING_FACTS 2
#define MAX_CONTRADICTING_FACTS 2
bool contradict(Fact **factArray, uint32_t n_facts);
#endif
#endif