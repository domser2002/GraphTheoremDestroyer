#include "test_gtd_common.h"
static uint8_t unit_test_counter = 0;
static uint8_t integration_test_counter = 0;
#define COMMON_UNIT_TEST_NUM 90
#define COMMON_INTEGRATION_TEST_NUM 0
typedef Function* (*function_binary_operation)(const Function *const, const Function *const);
typedef void (*function_constant_operation)(Function *, const int32_t);
typedef int8_t (*modifying_function_constant_operation_with_ret)(Function *, const int32_t);
typedef int32_t (*const_function_constant_operation_with_ret)(const Function *const, const int32_t);
typedef Function* (*function_unary_operation)(const Function *const);

typedef struct Function_Binary_Operation_Test_Case {
    Function *f1;
    Function *f2;
    Function *expected;
    function_binary_operation op;
} Function_Binary_Operation_Test_Case;

typedef struct Function_Constant_Operation_Test_Case {
    Function *f;
    int32_t c;
    Function *expected;
    function_constant_operation op;
} Function_Constant_Operation_Test_Case;

typedef struct Modifying_Function_Constant_Operation_With_Ret_Test_Case {
    Function *f;
    int32_t c;
    Function *expected;
    int8_t expected_ret;
    modifying_function_constant_operation_with_ret op;
} Modifying_Function_Constant_Operation_With_Ret_Test_Case;

typedef struct Const_Function_Constant_Operation_With_Ret_Test_Case {
    Function *f;
    int32_t c;
    int32_t expected_ret;
    const_function_constant_operation_with_ret op;
} Const_Function_Constant_Operation_With_Ret_Test_Case;

typedef struct Function_Unary_Operation_Test_Case {
    Function *f;
    Function *expected;
    function_unary_operation op;
} Function_Unary_Operation_Test_Case;

static void write_unit_test_num(void)
{
    printf("\rTEST %d/%d", unit_test_counter++, COMMON_UNIT_TEST_NUM);
    fflush(stdout);
}

static void write_integration_test_num(void)
{
    printf("\rTEST %d/%d", integration_test_counter++, COMMON_INTEGRATION_TEST_NUM);
    fflush(stdout);
}

static void run_function_binary_operation_test_case(Function_Binary_Operation_Test_Case test_case)
{
    write_unit_test_num();
    // Act
    Function *result = test_case.op(test_case.f1, test_case.f2);
    // Assert
    assert(compare_functions(result, test_case.expected) == 0);
    // Clean
    delete_function(result);
}

static void create_and_run_function_binary_operation_test_case(Function *f1, Function *f2, Function *expected, function_binary_operation op)
{
    // Arrange
    Function_Binary_Operation_Test_Case test_case = {
        .f1 = f1,
        .f2 = f2,
        .expected = expected,
        .op = op
    };
    // Act & Assert
    run_function_binary_operation_test_case(test_case);
    // Clean
    delete_function(f1);
    delete_function(f2);
    delete_function(expected);
}

static void run_function_constant_operation_test_case(Function_Constant_Operation_Test_Case test_case)
{
    write_unit_test_num();
    // Act
    test_case.op(test_case.f, test_case.c);
    // Assert
    assert(compare_functions(test_case.f, test_case.expected) == 0);
}

static void create_and_run_function_constant_operation_test_case(Function *f, int32_t c, Function *expected, function_constant_operation op)
{
    // Arrange
    Function_Constant_Operation_Test_Case test_case = {
        .f =f,
        .c = c,
        .expected = expected,
        .op = op
    };
    // Act & Assert
    run_function_constant_operation_test_case(test_case);
    // Clean
    delete_function(f);
    delete_function(expected);
}

static void run_modifying_function_constant_operation_with_ret_test_case(Modifying_Function_Constant_Operation_With_Ret_Test_Case test_case)
{
    write_unit_test_num();
    // Act
    int8_t ret = test_case.op(test_case.f, test_case.c);
    // Assert
    assert(ret == test_case.expected_ret);
    if(ret != OP_NOT_POSSIBLE)
        assert(compare_functions(test_case.f, test_case.expected) == 0);
}

static void create_and_run_modifying_function_constant_operation_with_ret_test_case(Function *f, int32_t c, Function *expected, int8_t expected_ret, modifying_function_constant_operation_with_ret op)
{
    // Arrange
    Modifying_Function_Constant_Operation_With_Ret_Test_Case test_case = {
        .f =f,
        .c = c,
        .expected = expected,
        .expected_ret = expected_ret,
        .op = op
    };
    // Act & Assert
    run_modifying_function_constant_operation_with_ret_test_case(test_case);
    // Clean
    delete_function(f);
    delete_function(expected);
}

static void run_const_function_constant_operation_with_ret_test_case(Const_Function_Constant_Operation_With_Ret_Test_Case test_case)
{
    write_unit_test_num();
    // Act
    int32_t ret = test_case.op(test_case.f, test_case.c);
    // Assert
    assert(ret == test_case.expected_ret);
}

static void create_and_run_const_function_constant_operation_with_ret_test_case(Function *f, int32_t c, int32_t expected_ret, const_function_constant_operation_with_ret op)
{
    // Arrange
    Const_Function_Constant_Operation_With_Ret_Test_Case test_case = {
        .f =f,
        .c = c,
        .expected_ret = expected_ret,
        .op = op
    };
    // Act & Assert
    run_const_function_constant_operation_with_ret_test_case(test_case);
    // Clean
    delete_function(f);
}

static void run_function_unary_operation_test_case(Function_Unary_Operation_Test_Case test_case)
{
    write_unit_test_num();
    // Act
    Function *result = test_case.op(test_case.f);
    // Assert
    assert(compare_functions(result, test_case.expected) == 0);
    // Clean
    delete_function(result);
}

static void create_and_run_function_unary_operation_test_case(Function *f, Function *expected, function_unary_operation op)
{
    // Arrange
    Function_Unary_Operation_Test_Case test_case = {
        .f = f,
        .expected = expected,
        .op = op
    };
    // Act & Assert
    run_function_unary_operation_test_case(test_case);
    // Clean
    delete_function(f);
    delete_function(expected);
}


/**
 * \brief function to run all tests for functions from the function.h file
*/
static void test_function(void)
{
    // add_functions
    create_and_run_function_binary_operation_test_case(create_function(0), create_function(0), create_function(0), add_functions);
    create_and_run_function_binary_operation_test_case(create_function(1), create_function(2), create_function(3), add_functions);
    // subtract_functions
    create_and_run_function_binary_operation_test_case(create_function(0), create_function(0), create_function(0), subtract_functions);
    create_and_run_function_binary_operation_test_case(create_function(2), create_function(1), create_function(1), subtract_functions);
    // multiply_functions
    create_and_run_function_binary_operation_test_case(create_function(0), create_function(0), create_function(0), multiply_functions);
    create_and_run_function_binary_operation_test_case(create_function(1), create_function(0), create_function(0), multiply_functions);    
    create_and_run_function_binary_operation_test_case(create_function(0), create_function(1), create_function(0), multiply_functions);
    create_and_run_function_binary_operation_test_case(create_function(2), create_function(1), create_function(2), multiply_functions);
    // divide_functions
    create_and_run_function_binary_operation_test_case(create_function(0), create_function(0), NULL, divide_functions);
    create_and_run_function_binary_operation_test_case(create_function(1), create_function(0), NULL, divide_functions);    
    create_and_run_function_binary_operation_test_case(create_function(0), create_function(1), create_function(0), divide_functions);
    create_and_run_function_binary_operation_test_case(create_function(2), create_function(1), create_function(2), divide_functions);
    // add_constant
    create_and_run_function_constant_operation_test_case(create_function(0), 0, create_function(0), add_constant);
    create_and_run_function_constant_operation_test_case(create_function(1), 2, create_function(3), add_constant);
    // subtract_constant
    create_and_run_function_constant_operation_test_case(create_function(0), 0, create_function(0), subtract_constant);
    create_and_run_function_constant_operation_test_case(create_function(2), 1, create_function(1), subtract_constant);
    // multiply_constant
    create_and_run_function_constant_operation_test_case(create_function(0), 0, create_function(0), multiply_constant);
    create_and_run_function_constant_operation_test_case(create_function(1), 0, create_function(0), multiply_constant);    
    create_and_run_function_constant_operation_test_case(create_function(0), 0, create_function(0), multiply_constant);
    create_and_run_function_constant_operation_test_case(create_function(2), 1, create_function(2), multiply_constant);
    // divide_constant
    create_and_run_modifying_function_constant_operation_with_ret_test_case(create_function(0), 0, NULL, OP_NOT_POSSIBLE, divide_constant);
    create_and_run_modifying_function_constant_operation_with_ret_test_case(create_function(1), 0, NULL, OP_NOT_POSSIBLE, divide_constant);    
    create_and_run_modifying_function_constant_operation_with_ret_test_case(create_function(0), 1, create_function(0), 0, divide_constant);
    create_and_run_modifying_function_constant_operation_with_ret_test_case(create_function(2), 1, create_function(2), 0, divide_constant);
    // mod_function
    create_and_run_const_function_constant_operation_with_ret_test_case(create_function(0), 2, 0, mod_function);
    create_and_run_const_function_constant_operation_with_ret_test_case(create_function(5), 2, 1, mod_function);
    // sqrt_function
    create_and_run_function_unary_operation_test_case(create_function(0),create_function(0),sqrt_function);
    create_and_run_function_unary_operation_test_case(create_function(4),create_function(2),sqrt_function);
}

static void run_get_param_count_test(FactType type, uint8_t expected)
{
    write_unit_test_num();
    // Act
    uint8_t ret = get_params(type, NULL, NULL);
    // Assert
    assert(ret == expected);
}

static void run_get_fact_str_test(Fact *fact, char expected[MAX_FACT_STR_LEN])
{
    write_unit_test_num();
    // Act
    char *result = get_fact_str(fact);
    // Assert
    assert(strcmp(result,expected) == 0);
    // Clean
    gtd_free(result);
}

static void create_fact_and_run_get_fact_str_test(char expected[MAX_FACT_STR_LEN], FactType type, uint8_t param_count, ...)
{
    // Arrange
    Function **params = (Function**)gtd_malloc(param_count * sizeof(Function*));
    va_list args;
    va_start(args, param_count);
    for(uint8_t i=0;i<param_count;i++)
    {
        params[i] = va_arg(args,Function*);
    }
    va_end(args);
    Fact *fact = create_fact(type, params);
    // Act & Assert
    run_get_fact_str_test(fact, expected);
    // Clean
    delete_fact(fact);
    gtd_free(params);
}

static Fact *create_fact_wrapper(FactType type, uint8_t param_count, ...)
{
    Function **params = (Function**)gtd_malloc(param_count * sizeof(Function*));
    va_list args;
    va_start(args, param_count);
    for(uint8_t i=0;i<param_count;i++)
    {
        params[i] = va_arg(args,Function*);
    }
    va_end(args);
    Fact *fact = create_fact(type, params);
    gtd_free(params);
    return fact;
}

static void run_equal_facts_test(Fact *fact1, Fact *fact2, bool expected)
{
    write_unit_test_num();
    // Act
    bool result = equal_facts(fact1, fact2);
    // Assert
    assert(result == expected);
}

/**
 * \brief function to run all tests for functions from the fact.h file
*/
static void test_fact(void)
{
    // get_param_count
    run_get_param_count_test(IsConnectedFact, 0);
    run_get_param_count_test(IsTreeFact, 0);
    run_get_param_count_test(IsPlanarFact,0);
    run_get_param_count_test(IsCycleFact,0);
    run_get_param_count_test(IsCycleComplementFact,0);
    run_get_param_count_test(HasNoCyclesFact,0);
    run_get_param_count_test(IstnaryTreeFact,1);
    run_get_param_count_test(IsPartiteFact,1);
    run_get_param_count_test(VertexCountFact,1);
    run_get_param_count_test(MinVertexCountFact,1);
    run_get_param_count_test(MaxVertexCountFact,1);
    run_get_param_count_test(EdgeCountFact,1);
    run_get_param_count_test(MinEdgeCountFact,1);
    run_get_param_count_test(MaxEdgeCountFact,1);
    run_get_param_count_test(TreeHeightFact,1);
    run_get_param_count_test(MinTreeHeightFact,1);
    run_get_param_count_test(MaxTreeHeightFact,1);
    run_get_param_count_test(HasCycleFact,1);
    run_get_param_count_test(HasNoCycleFact,1);
    run_get_param_count_test(HasInducedCycleFact,1);
    run_get_param_count_test(HasNoInducedCycleFact,1);
    run_get_param_count_test(HasMinorCycleFact,1);
    run_get_param_count_test(HasNoMinorCycleFact,1);
    run_get_param_count_test(HasPathFact,1);
    run_get_param_count_test(HasNoPathFact,1);
    run_get_param_count_test(HasInducedPathFact,1);
    run_get_param_count_test(HasNoInducedPathFact,1);
    run_get_param_count_test(HasMinorPathFact,1);
    run_get_param_count_test(HasNoMinorPathFact,1);
    run_get_param_count_test(HasCliqueFact,1);
    run_get_param_count_test(HasNoCliqueFact,1);
    run_get_param_count_test(HasMinorCliqueFact,1);
    run_get_param_count_test(HasNoMinorCliqueFact,1);
    run_get_param_count_test(HasCompletePartiteFact,3);
    run_get_param_count_test(HasNoCompletePartiteFact,3);
    run_get_param_count_test(HasInducedCompletePartiteFact,3);
    run_get_param_count_test(HasNoInducedCompletePartiteFact,3);
    run_get_param_count_test(HasMinorCompletePartiteFact,3);
    run_get_param_count_test(HasNoMinorCompletePartiteFact,3);
    // get_fact_str
    char str[MAX_FACT_STR_LEN];
    sprintf(str, "Graph is connected");
    create_fact_and_run_get_fact_str_test(str, IsConnectedFact, 0);
    sprintf(str, "Graph is a tree");
    create_fact_and_run_get_fact_str_test(str, IsTreeFact, 0);
    sprintf(str, "Graph is planar");
    create_fact_and_run_get_fact_str_test(str, IsPlanarFact, 0);
    sprintf(str, "Graph is a cycle");
    create_fact_and_run_get_fact_str_test(str, IsCycleFact, 0);
    sprintf(str, "Graph is a complement of a cycle");
    create_fact_and_run_get_fact_str_test(str, IsCycleComplementFact, 0);
    sprintf(str, "Graph has no cycles");
    create_fact_and_run_get_fact_str_test(str, HasNoCyclesFact, 0);
    sprintf(str, "Graph is a 2-nary tree");
    create_fact_and_run_get_fact_str_test(str, IstnaryTreeFact, 1, create_function(2));
    sprintf(str, "Graph is a path");
    create_fact_and_run_get_fact_str_test(str, IstnaryTreeFact, 1, create_function(1));
    sprintf(str, "Graph is 2-partite");
    create_fact_and_run_get_fact_str_test(str, IsPartiteFact, 1, create_function(2));
    sprintf(str, "Graph is 3-partite");
    create_fact_and_run_get_fact_str_test(str, IsPartiteFact, 1, create_function(3));
    sprintf(str, "Graph has 2 vertices");
    create_fact_and_run_get_fact_str_test(str, VertexCountFact, 1, create_function(2));
    sprintf(str, "Graph has 3 vertices");
    create_fact_and_run_get_fact_str_test(str, VertexCountFact, 1, create_function(3));
    sprintf(str, "Graph contains K_1,1 as a subgraph");
    create_fact_and_run_get_fact_str_test(str, HasCompletePartiteFact, 3, create_function(2), create_function(1), create_function(1));
    // TODO: cover the rest of types
    // equal_facts
    // 0 params, equal types
    Fact *fact1 = create_fact_wrapper(IsConnectedFact,0);
    Fact *fact2 = create_fact_wrapper(IsConnectedFact,0);
    run_equal_facts_test(fact1, fact2, true);
    delete_fact(fact1);
    delete_fact(fact2);
    // 0 params, not equal types
    fact1 = create_fact_wrapper(IsConnectedFact,0);
    fact2 = create_fact_wrapper(IsTreeFact,0);
    run_equal_facts_test(fact1, fact2, false);
    delete_fact(fact1);
    delete_fact(fact2);
    // 1 param, equal types and param
    fact1 = create_fact_wrapper(MaxVertexCountFact, 1, create_function(1));
    fact2 = create_fact_wrapper(MaxVertexCountFact, 1, create_function(1));
    run_equal_facts_test(fact1, fact2, true);
    delete_fact(fact1);
    delete_fact(fact2);
    // 1 param, not equal types, equal param
    fact1 = create_fact_wrapper(MaxVertexCountFact, 1, create_function(1));
    fact2 = create_fact_wrapper(VertexCountFact, 1, create_function(1));
    run_equal_facts_test(fact1, fact2, false);
    delete_fact(fact1);
    delete_fact(fact2);
    // 1 param, equal types, not equal param
    fact1 = create_fact_wrapper(MaxVertexCountFact, 1, create_function(1));
    fact2 = create_fact_wrapper(MaxVertexCountFact, 1, create_function(2));
    run_equal_facts_test(fact1, fact2, false);
    delete_fact(fact1);
    delete_fact(fact2);
    // 1 param, not equal types, not equal param
    fact1 = create_fact_wrapper(MaxVertexCountFact, 1, create_function(1));
    fact2 = create_fact_wrapper(VertexCountFact, 1, create_function(2));
    run_equal_facts_test(fact1, fact2, false);
    delete_fact(fact1);
    delete_fact(fact2);
    // 3 params, equal types, equal params
    fact1 = create_fact_wrapper(HasCompletePartiteFact, 3, create_function(2), create_function(1), create_function(1));
    fact2 = create_fact_wrapper(HasCompletePartiteFact, 3, create_function(2), create_function(1), create_function(1));
    run_equal_facts_test(fact1, fact2, true);
    delete_fact(fact1);
    delete_fact(fact2);
    // 3 params, not equal types, equal params
    fact1 = create_fact_wrapper(HasCompletePartiteFact, 3, create_function(2), create_function(1), create_function(1));
    fact2 = create_fact_wrapper(HasNoCompletePartiteFact, 3, create_function(2), create_function(1), create_function(1));
    run_equal_facts_test(fact1, fact2, false);
    delete_fact(fact1);
    delete_fact(fact2);
    // 3 params, equal types, not equal params
    fact1 = create_fact_wrapper(HasCompletePartiteFact, 3, create_function(2), create_function(2), create_function(1));
    fact2 = create_fact_wrapper(HasCompletePartiteFact, 3, create_function(2), create_function(1), create_function(1));
    run_equal_facts_test(fact1, fact2, false);
    delete_fact(fact1);
    delete_fact(fact2);
    // 3 params, not equal types, not equal params
    fact1 = create_fact_wrapper(HasCompletePartiteFact, 3, create_function(2), create_function(2), create_function(1));
    fact2 = create_fact_wrapper(HasNoCompletePartiteFact, 3, create_function(2), create_function(1), create_function(1));
    run_equal_facts_test(fact1, fact2, false);
    delete_fact(fact1);
    delete_fact(fact2);
}


void gtd_common_unit_tests(void)
{
    printf("RUN UNIT TESTS\n");
    write_unit_test_num();
    test_function();
    test_fact();
}

void gtd_common_integration_tests(void)
{
    printf("\nRUN INTEGRATION TESTS\n");
    write_integration_test_num();
}