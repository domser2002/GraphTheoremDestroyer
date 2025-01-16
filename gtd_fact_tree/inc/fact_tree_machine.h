/**
 * \file fact_tree_machine.h
 * \brief header including all necessary functions to create, delete, execute and obtain proof
 *  from a machine, which implements logic to attempt to prove a theorem
 */
#ifndef FACT_TREE_MACHINE_H
#define FACT_TREE_MACHINE_H
/** \cond IS_FACT_TREE_COMPONENT */
#ifdef IS_FACT_TREE_COMPONENT
/** \endcond */
#include "fact_tree.h"
#include "common.h"
#include "fact.h"
#include "contradiction.h"
#include "implication.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct FactTreeMachine FactTreeMachine;

/**
 * \brief initialize new proof machine based on initial fact tree
 * \param FactTree initial fact tree
 * \returns newly created machine
*/
FactTreeMachine *init_machine(FactTree *FactTree);

/**
 * \brief delete proof machine
 * \param machine machine to delete
*/
void delete_machine(FactTreeMachine *machine);

/**
 * \brief perform proof machine execution
 * \details machine tries to find predefined contradiciton in the given tree of facts, 
 * if it is not found, it looks for one of the predefined implications, adds new facts to a tree
 * and executes recursively until contradiction is found or no more facts can be added 
 * \param machine machine to be executed
 * \note function does not return, it only updates fields in machine class, proof can be written
 *  using write_proof function
*/
void execute(FactTreeMachine *machine);

/**
 * \brief function to write a full proof of a theorem
 * \param machine executed machine
 * \param output output file for a proof
 * \returns true on success, false otherwsie
 */
bool write_proof(FactTreeMachine *machine, FILE *output);

/**
 * \brief function to write a deduction of a specific fact found by machine during execution
 * \param FactTree fact tree of an executed machine
 * \param idx index of a Fact, which deduction is requested in a FactTree
 * \param output output file for a proof
 * \returns true on success, false otherwsie
 */
void write_deduction(FactTree *FactTree, uint32_t idx, FILE *output);
// getters
bool get_contradiction_found(FactTreeMachine *machine);
FactTree *get_fact_tree(FactTreeMachine *machine);
Fact **get_contradicting_facts(FactTreeMachine *machine, uint32_t *contradicting_count);
#endif
#endif