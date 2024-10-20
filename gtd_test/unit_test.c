#include <assert.h>
#include <stdio.h>
#include "test_fact_tree.h"
#include "test_physical_graph.h"
#include "test_generative_proof_machine.h"
#define TEST_MODE

// int sum(int a,int b)
// {
//     return a+b;
// }

// void test_sum(void) {
//     // Arrange: No specific setup needed here
//     // Act & Assert:
//     assert(sum(1, 2) == 3);    // Passes
//     assert(sum(-1, -1) == -2); // Passes
//     assert(sum(0, 0) == 0);    // Passes
//     assert(sum(100, 200) == 300); // Passes
// }

int main(void) {
    test_fact_tree();
    test_physical_graph();
    test_generative_proof_machine();
    printf("All tests passed!\n");
    return 0;
}
