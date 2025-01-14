/**
 * \file contradiction.h
 * \brief header containing only one function allowing to find contradictions 
 */
#ifndef CONTRADICTION_H
#define CONTRADICTION_H
#ifdef IS_FACT_TREE_COMPONENT
#include "fact.h"
#include "function.h"
#define MIN_CONTRADICTING_FACTS 2
#define MAX_CONTRADICTING_FACTS 3

/**
 * \brief Function which checks if facts from factArray are in one of the predefined contradictions
 * \param factArray array of n_facts Fact objects, fact types should not duplicate, if they do the behaviour is undefined
 * \param n_facts number of facts in array, should be <= MAX_CONTRADICTING_FACTS
 * \return true if facts are in one of the predefined contradictions, false otherwise
 */
bool contradict(Fact **factArray, uint32_t n_facts);
#endif
#endif