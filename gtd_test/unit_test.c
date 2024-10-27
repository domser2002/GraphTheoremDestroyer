#include <assert.h>
#include <stdio.h>
#include "test_fact_tree.h"

int main(void) {
    test_mode = true;
    test_fact_tree();
    printf("All tests passed!\n");
    return 0;
}
