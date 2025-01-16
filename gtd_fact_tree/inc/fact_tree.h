/**
 * \file fact_tree.h
 * \brief header containg implementation of FactTree data structure, main structure used by
 *  GTD_FACT_TREE module
 */
#ifndef FACT_TREE_H
#define FACT_TREE_H
/** \cond IS_FACT_TREE_COMPONENT */
#ifdef IS_FACT_TREE_COMPONENT
/** \endcond */
#include <stdint.h>
#include <stdbool.h>
#include "fact.h"
#include "common.h"

typedef struct FactTree
{
    uint32_t fact_count;
    uint32_t **parents; 
    uint8_t *parent_count;
    Fact **facts;
    Fact ***original_facts;
} FactTree;

/**
 * \brief function to create an initial FactTree with isolated facts
 * \param fact_count number of facts
 * \param facts pointer to the array of pointers to facts
 * \returns pointer to a newly created FactTree
*/
FactTree *construct(uint32_t fact_count, Fact ***facts);

/**
 * \brief function to destruct FactTree
 * \param ft FactTree to destruct
*/
void destruct(FactTree* ft);

/**
 * \brief function to add new fact to the FactTree, unless it already exists
 * \param ft - fact tree
 * \param parent_idxs - array of parents of the new Fact
 * \param parent_count - number of parents of the new Fact
 * \param new_fact - fact to be added
*/
bool add_fact(FactTree *ft, uint32_t *parent_idxs, uint8_t parent_count, Fact *new_fact);
#endif
#endif