/**
 * \file test_gtd_main.c
 * \brief implementation of functions from test_gtd_main.h
 */
#include "test_gtd_main.h"
static uint8_t unit_test_counter = 0;
static uint8_t integration_test_counter = 0;
#define MAIN_UNIT_TEST_NUM 4
#define MAIN_INTEGRATION_TEST_NUM 3

static void write_unit_test_num(void)
{
    printf("\rTEST %d/%d", unit_test_counter++, MAIN_UNIT_TEST_NUM);
    fflush(stdout);
}

static void write_integration_test_num(void)
{
    printf("\rTEST %d/%d", integration_test_counter++, MAIN_INTEGRATION_TEST_NUM);
    fflush(stdout);
}


static void run_get_module_args_test(const char *pathname, ModuleArgs *expected)
{
    write_unit_test_num();
    // Act
    ModuleArgs *args = get_module_args_from_json(pathname);
    // Assert
    assert(expected->fact_count == args->fact_count);
    for(uint32_t i=0; i<args->fact_count; i++)
    {
        assert(equal_facts(args->fact_array[i], expected->fact_array[i]) == true);
    }
}

static void test_get_module_args_from_json(void)
{
    Function *params1[1];
    Function *params3[3];
    ModuleArgs *expected = (ModuleArgs *)gtd_calloc(1, sizeof(ModuleArgs));
    expected->fact_count = 0;
    expected->fact_array = NULL;
    run_get_module_args_test("gtd_main/test/empty.json", expected);

    expected = (ModuleArgs *)gtd_calloc(1, sizeof(ModuleArgs));
    expected->fact_count = 5;
    expected->fact_array = (Fact **)gtd_malloc(5 * sizeof(Fact *));
    expected->fact_array[0] = create_fact(HasNoCyclesFact, NULL);
    params3[0] = create_constant_integer_function(2);
    params3[1] = create_constant_integer_function(1);
    params3[2] = create_constant_integer_function(4);
    expected->fact_array[1] = create_fact(HasNoInducedCompletePartiteFact, params3);
    params1[0] = create_constant_integer_function(4);
    expected->fact_array[2] = create_fact(HasNoInducedPathFact, params1);
    params1[0] = create_constant_integer_function(8);
    expected->fact_array[3] = create_fact(MinVertexCountFact, params1);
    expected->fact_array[4] = create_fact(IsConnectedFact, NULL);
    run_get_module_args_test("gtd_main/test/fact_tree.json", expected);

    expected = (ModuleArgs *)gtd_calloc(1, sizeof(ModuleArgs));
    expected->fact_count = 6;
    expected->fact_array = (Fact **)gtd_malloc(6 * sizeof(Fact *));
    params1[0] = create_constant_integer_function(4);
    expected->fact_array[0] = create_fact(HasNoCycleFact, params1);
    params1[0] = create_constant_integer_function(8);
    expected->fact_array[1] = create_fact(HasNoCycleFact, params1);
    params1[0] = create_constant_integer_function(8);
    expected->fact_array[2] = create_fact(HasNoInducedPathFact, params1);
    params1[0] = create_constant_integer_function(3);
    expected->fact_array[3] = create_fact(MinVertexDegreeFact, params1);
    params1[0] = create_constant_integer_function(3);
    expected->fact_array[4] = create_fact(HasNoUnknownEdgesFact, params1);
    params1[0] = create_constant_integer_function(7);
    expected->fact_array[5] = create_fact(HasInducedCycleFact, params1);
    run_get_module_args_test("gtd_main/test/erdos-gyarvas.json", expected);

    expected = (ModuleArgs *)gtd_calloc(1, sizeof(ModuleArgs));
    expected->fact_count = 11;
    expected->fact_array = (Fact **)gtd_malloc(11 * sizeof(Fact *));
    params1[0] = create_constant_integer_function(4);
    expected->fact_array[0] = create_fact(HasNoCycleFact, params1);
    params1[0] = create_constant_integer_function(8);
    expected->fact_array[1] = create_fact(HasNoCycleFact, params1);
    params1[0] = create_constant_integer_function(8);
    expected->fact_array[2] = create_fact(HasNoInducedPathFact, params1);
    params1[0] = create_constant_integer_function(3);
    expected->fact_array[3] = create_fact(MinVertexDegreeFact, params1);
    params1[0] = create_constant_integer_function(3);
    expected->fact_array[4] = create_fact(HasNoUnknownEdgesFact, params1);
    params1[0] = create_constant_integer_function(7);
    expected->fact_array[5] = create_fact(HasInducedCycleFact, params1);
    expected->fact_array[6] = create_fact(HasNoCyclesFact, NULL);
    params3[0] = create_constant_integer_function(2);
    params3[1] = create_constant_integer_function(1);
    params3[2] = create_constant_integer_function(4);
    expected->fact_array[7] = create_fact(HasNoInducedCompletePartiteFact, params3);
    params1[0] = create_constant_integer_function(4);
    expected->fact_array[8] = create_fact(HasNoInducedPathFact, params1);
    params1[0] = create_constant_integer_function(8);
    expected->fact_array[9] = create_fact(MinVertexCountFact, params1);
    expected->fact_array[10] = create_fact(IsConnectedFact, NULL);
    run_get_module_args_test("gtd_main/test/both.json", expected);    
}

void gtd_main_unit_tests(void)
{
    printf("RUN UNIT TESTS\n");
    write_unit_test_num();
    test_get_module_args_from_json();
}

static void run_run_modules_test(ModuleArgs *args, char *expected_file_path)
{
    GTD_UNUSED(expected_file_path);
    write_integration_test_num();
    // Act
    run_modules(args);
    // Assert 

}

static void test_run_modules(void)
{
    Function *params1[1];
    Function *params3[3];
    ModuleArgs *expected = (ModuleArgs *)gtd_calloc(1, sizeof(ModuleArgs));
    sprintf(expected->out_file_path, "test.txt");
    run_run_modules_test(expected, "gtd_main/test/not_found.txt");
    remove(expected->out_file_path);

    expected = (ModuleArgs *)gtd_calloc(1, sizeof(ModuleArgs));
    sprintf(expected->out_file_path, "test.txt");
    expected->fact_count = 5;
    expected->fact_array = (Fact **)gtd_malloc(5 * sizeof(Fact *));
    expected->fact_array[0] = create_fact(HasNoCyclesFact, NULL);
    params3[0] = create_constant_integer_function(2);
    params3[1] = create_constant_integer_function(1);
    params3[2] = create_constant_integer_function(4);
    expected->fact_array[1] = create_fact(HasNoInducedCompletePartiteFact, params3);
    params1[0] = create_constant_integer_function(4);
    expected->fact_array[2] = create_fact(HasNoInducedPathFact, params1);
    params1[0] = create_constant_integer_function(8);
    expected->fact_array[3] = create_fact(MinVertexCountFact, params1);
    expected->fact_array[4] = create_fact(IsConnectedFact, NULL);
    run_run_modules_test(expected, "gtd_main/test/fact_tree.txt");
    remove(expected->out_file_path);

    expected = (ModuleArgs *)gtd_calloc(1, sizeof(ModuleArgs));
    sprintf(expected->out_file_path, "test.txt");
    expected->fact_count = 6;
    expected->fact_array = (Fact **)gtd_malloc(6 * sizeof(Fact *));
    params1[0] = create_constant_integer_function(4);
    expected->fact_array[0] = create_fact(HasNoCycleFact, params1);
    params1[0] = create_constant_integer_function(8);
    expected->fact_array[1] = create_fact(HasNoCycleFact, params1);
    params1[0] = create_constant_integer_function(8);
    expected->fact_array[2] = create_fact(HasNoInducedPathFact, params1);
    params1[0] = create_constant_integer_function(3);
    expected->fact_array[3] = create_fact(MinVertexDegreeFact, params1);
    params1[0] = create_constant_integer_function(3);
    expected->fact_array[4] = create_fact(HasNoUnknownEdgesFact, params1);
    params1[0] = create_constant_integer_function(7);
    expected->fact_array[5] = create_fact(HasInducedCycleFact, params1);
    run_run_modules_test(expected, "gtd_main/test/erdos-gyarvas.txt");
    remove(expected->out_file_path);
}

void gtd_main_integration_tests(void)
{
    printf("\nRUN INTEGRATION TESTS\n");
    write_integration_test_num();
    test_run_modules();
}