#include <assert.h>
#include <stdio.h>
#include "test_gtd_common.h"
#include "test_gtd_main.h"
#include "test_gtd_fact_tree.h"
#include "test_gtd_generative.h"
#define TEST_MODE

int main(void) {
    test_mode = true;
    printf("\nTESTING GTD_COMMON\n");
    gtd_common_unit_tests();
    gtd_common_integration_tests();
    printf("\nTESTING GTD_MAIN\n");
    gtd_main_unit_tests();
    gtd_main_integration_tests();
    printf("\nTESTING GTD_FACT_TREE\n");
    gtd_fact_tree_unit_tests();
    gtd_fact_tree_integration_tests();
    printf("\nTESTING GTD_GENERATIVE\n");
    gtd_generative_unit_tests();
    gtd_generative_integration_tests();
    printf("All tests passed!\n");
    return 0;
}
