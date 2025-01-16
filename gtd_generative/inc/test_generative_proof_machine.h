/**
 * \file test_generative_proof_machine.h
 * \brief header to run all tests for GenerativeProofMachine
 */
#ifndef TEST_GENERATIVE_PROOF_MACHINE_H
#define TEST_GENERATIVE_PROOF_MACHINE_H
void test_generative_proof_machine(void);
void run_generative_proof_machine_tests(void);
void test_machine_creation_and_deletion(void);
void test_max_degree_restriction(void);
void test_no_k_cycle_restriction(void);
void test_no_induced_path_k_restriction(void);
void test_erdos_gyarfas_case(void);
void test_minimum_degree_restriction(void);
void test_erdos_gyarfas_pk_free(int k, int max_vertices, int max_depth, int save_to_file);

#endif
