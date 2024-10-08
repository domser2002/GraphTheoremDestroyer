#include <assert.h>
#include <stdio.h>

int sum(int a,int b)
{
    return a+b;
}

void test_sum(void) {
    // Arrange: No specific setup needed here
    // Act & Assert:
    assert(sum(1, 2) == 3);    // Passes
    assert(sum(-1, -1) == -2); // Passes
    assert(sum(0, 0) == 0);    // Passes
    assert(sum(100, 200) == 300); // Passes
}

int main(void) {
    test_sum();
    printf("All tests passed!\n");
    return 0;
}
