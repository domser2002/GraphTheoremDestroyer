/**
 * \file implication.h
 * \brief header containing only one function allowing to find implications 
 */
#ifndef IMPLICATION_H
#define IMPLICATION_H
/** \cond IS_FACT_TREE_COMPONENT */
#ifdef IS_FACT_TREE_COMPONENT
/** \endcond */
#define MAX_LEFT_SIDE_FACTS 2
#define MIN_LEFT_SIDE_FACTS 1
#include "fact.h"
#include <string.h>

/**
 * \brief function that checks if array of facts is the left side of a known implication
 *  and returns right side of it in that case
 * \param factArray array of facts (potential left side)
 * \param n_facts number of elements in factArray
 * \param count pointer to return number of elements on the right side
 * \return right side of implication or NULL if there is no implication with that left side defined
 */
Fact **implies(Fact **factArray, uint32_t n_facts, int *count);
#endif
#endif