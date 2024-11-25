#ifndef TEST_GENERATIVE_PROOF_MACHINE_H
#define TEST_GENERATIVE_PROOF_MACHINE_H

#include "physical_graph.h"
#include "generative_restriction.h"
#include "generative_proof_machine.h"
#include <assert.h>

void test_generative_proof_machine(void);

void creation_and_deletion(void);

void test_max_degree(void);

void execute_small_machine(void);

void execute_small_machine2(void);

void test_no_ck_restriction(void);

void test_no_induced_pk_restriction(void);

void test_erdos_gyarfas_subcase(void);

void test_min_degree(void);

void test_check_edge(void);


#endif