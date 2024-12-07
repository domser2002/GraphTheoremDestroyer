#include <assert.h>
#include <stdio.h>
#include "test_fact_tree.h"
#include "test_physical_graph.h"
#include "test_generative_proof_machine.h"
#define TEST_MODE

int main(void) {
    test_mode = true;
    test_fact_tree();
    test_physical_graph();
    test_generative_proof_machine();
    printf("All tests passed!\n");
    return 0;
}
