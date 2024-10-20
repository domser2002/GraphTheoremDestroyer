#include <assert.h>
#include <stdio.h>
#include "test_fact_tree.h"
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
    printf("All tests passed!\n");
    return 0;
}
