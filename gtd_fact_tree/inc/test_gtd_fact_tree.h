/**
 * \file test_gtd_fact_tree.h
 * \brief header to run all tests for gtd_fact_tree
 */
#ifndef TEST_FACT_H
#define TEST_FACT_H
#define IS_FACT_TREE_COMPONENT
#include <assert.h>
#include <string.h>
#include "fact.h"
#include "contradiction.h"
#include "implication.h"
#include "function.h"
#include "fact_tree.h"
#include "fact_tree_machine.h"
void gtd_fact_tree_unit_tests(void);
void gtd_fact_tree_integration_tests(void);
#endif