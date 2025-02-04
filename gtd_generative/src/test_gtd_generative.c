/**
 * \file test_gtd_generative.c
 * \brief implementation of functions from test_gtd_generative.h
 */
#include "test_gtd_generative.h"

void gtd_generative_unit_tests(void)
{
    test_physical_graph();
}

void gtd_generative_integration_tests(void)
{
    test_generative_proof_machine();
}