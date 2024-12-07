#ifndef TEST_GENERATIVE_PROOF_MACHINE_H
#define TEST_GENERATIVE_PROOF_MACHINE_H

#include "physical_graph.h"
#include "generative_restriction.h"
#include "generative_proof_machine.h"
#include <assert.h>

void test_generative_proof_machine(void);
void run_generative_proof_machine_tests(void);
void test_machine_creation_and_deletion(void);
void test_max_degree_restriction(void);
void test_no_k_cycle_restriction(void);
void test_no_induced_path_k_restriction(void);
void test_erdos_gyarfas_case(void);
void test_minimum_degree_restriction(void);
void erdos_gyarfas_p7_free(void);
void erdos_gyarfas_p8_free(void);
void erdos_gyarfas_p9_free(void);

#endif