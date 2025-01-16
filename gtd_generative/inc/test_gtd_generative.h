/**
 * \file test_gtd_generative.h
 * \brief header to run all tests for gtd_generative
 */
#ifndef TEST_GTD_GENERATIVE_H
#define TEST_GTD_GENERATIVE_H
#define IS_GENERATIVE_MODULE_COMPONENT
#include <assert.h>
#include <string.h>
#include "test_physical_graph.h"
#include "test_generative_proof_machine.h"
void gtd_generative_unit_tests(void);
void gtd_generative_integration_tests(void);
#endif