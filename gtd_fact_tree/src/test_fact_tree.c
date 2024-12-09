#include "test_fact_tree.h"
static uint8_t counter = 1;
typedef Function* (*function_binary_operation)(const Function *const, const Function *const);
typedef void (*function_constant_operation)(Function *, const int32_t);
typedef int8_t (*modifying_function_constant_operation_with_ret)(Function *, const int32_t);
typedef int32_t (*const_function_constant_operation_with_ret)(const Function *const, const int32_t);
typedef Function* (*function_unary_operation)(const Function *const);

typedef struct Modular_Fact_Array {
    FactType *types;
    Function ***params;
    uint8_t *params_count;
    uint32_t fact_count;
} Modular_Fact_Array;

typedef struct Implies_Test_Case {
    Modular_Fact_Array left;
    Modular_Fact_Array right;
} Implies_Test_Case;

typedef struct Contradict_Test_Case {
    Modular_Fact_Array facts;
    bool expected;
} Contradict_Test_Case;

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

static Fact **create_fact_array_from_modular(Modular_Fact_Array mfa)
{
    Fact **factArray = (Fact**)gtd_malloc(mfa.fact_count * sizeof(Fact*));
    for(uint32_t i=0;i<mfa.fact_count;i++)
        factArray[i] = create_fact(mfa.types[i],mfa.params[i],mfa.params_count[i]);
    return factArray;
}

static void delete_fact_array(Fact **factArray,int fact_count)
{
    for(int i=0;i<fact_count;i++)
        delete_fact(factArray[i]);
    gtd_free(factArray);
}

static void single_contradict_test(Fact **factArray, uint32_t fact_count, bool expected)
{
    printf("TEST %d\n", counter++);
    // Arrange
    // Act
    bool result = contradict(factArray, fact_count);
    // Assert
    assert(result == expected);
}

static void run_contradict_test_case(Contradict_Test_Case test_case)
{
    Fact **factArray = create_fact_array_from_modular(test_case.facts);
    single_contradict_test(factArray,test_case.facts.fact_count,test_case.expected);
    delete_fact_array(factArray,test_case.facts.fact_count);
}

static void create_and_run_contradict_test_case(FactType *types, Function ***params, uint8_t *params_count, uint32_t fact_count, bool expected)
{
    Contradict_Test_Case test_case = {
        .facts = {
            .types = types,
            .params = params,
            .params_count = params_count,
            .fact_count = fact_count
        },
        .expected = expected
    };
    run_contradict_test_case(test_case);
}

/**
 * \brief function to run all tests for contradict method
 */
static void test_contradict(void)
{
    FactType types1[1]; // use if 1 fact
    FactType types2[2]; // use if 2 facts
    FactType types3[3]; // use if 3 facts
    Function *params1_1[1]; // use for fact 1 if it has 1 param 
    Function *params1_2[2]; // use for fact 1 if it has 2 params 
    Function *params1_3[3]; // use for fact 1 if it has 3 params 
    Function *params2_1[1]; // use for fact 2 if it has 1 param
    Function *params2_2[2]; // use for fact 2 if it has 2 params
    Function *params2_3[3]; // use for fact 2 if it has 3 params
    Function *params3_1[1]; // use for fact 3 if it has 1 param 
    Function *params3_2[2]; // use for fact 3 if it has 2 params 
    Function *params3_3[3]; // use for fact 3 if it has 3 params
    Function *params_3[3];
    Function **params1[1]; // use if 1 fact
    Function **params2[2]; // use if 2 facts
    Function **params3[3]; // use if 3 facts
    uint8_t param_count1[1];  // use if 1 fact
    uint8_t param_count2[2]; // use if 2 facts
    uint8_t param_count3[3]; // use if 3 facts

    GTD_UNUSED(params1_2);
    GTD_UNUSED(params1_3);
    GTD_UNUSED(params2_2);
    GTD_UNUSED(params3_1);
    GTD_UNUSED(params3_2);
    GTD_UNUSED(params3_3);
    GTD_UNUSED(params_3);
    // Case 1 - no facts case
    create_and_run_contradict_test_case(NULL,NULL,NULL,0,false);
    // Case 2 - only one fact case
    types1[0] = 0;
    params1[0] = NULL;
    param_count1[0] = 0;
    create_and_run_contradict_test_case(types1, params1, param_count1, 1, false);
    // Case 3 - contradiction type 1 should occur
    types2[0] = MaxVertexCountFact;
    types2[1] = MinEdgeCountFact;
    params1_1[0] = create_function(1);
    params2_1[0] = create_function(1);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, true);
    // Case 4 - contradiction type 1 should not occur due to params
    types2[0] = MaxVertexCountFact;
    types2[1] = MinEdgeCountFact;
    params1_1[0] = create_function(0);
    params2_1[0] = create_function(0);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 5 - contradiction type 1 should not occur due to types
    types2[0] = MaxVertexCountFact;
    types2[1] = MinTreeHeightFact;
    params1_1[0] = create_function(1);
    params2_1[0] = create_function(1);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 6 - contradiction type 2 should occur
    types2[0] = MinVertexCountFact;
    types2[1] = MaxVertexCountFact;
    params1_1[0] = create_function(2);
    params2_1[0] = create_function(1);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, true);
    // Case 7 - contradiction type 2 should not occur due to params
    types2[0] = MinVertexCountFact;
    types2[1] = MaxVertexCountFact;
    params1_1[0] = create_function(1);
    params2_1[0] = create_function(1);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 8 - contradiction type 2 should not occur due to types
    types2[0] = MinVertexCountFact;
    types2[1] = MaxTreeHeightFact;
    params1_1[0] = create_function(2);
    params2_1[0] = create_function(1);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 9 - contradiction type 3 should occur
    types2[0] = MinEdgeCountFact;
    types2[1] = MaxEdgeCountFact;
    params1_1[0] = create_function(2);
    params2_1[0] = create_function(1);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, true);
    // Case 10 - contradiction type 3 should not occur due to params
    types2[0] = MinEdgeCountFact;
    types2[1] = MaxEdgeCountFact;
    params1_1[0] = create_function(1);
    params2_1[0] = create_function(1);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 11 - contradiction type 3 should not occur due to types
    types2[0] = MinEdgeCountFact;
    types2[1] = MaxTreeHeightFact;
    params1_1[0] = create_function(2);
    params2_1[0] = create_function(1);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 12 - contradiction type 4 should occur
    // Case 13 - contradiction type 4 should not occur due to params
    // Case 14 - contradiction type 4 should not occur due to types
    // CONTRADICTION NOT IMPLEMENTED YET
    // Case 15 - contradiction type 5 should occur
    types2[0] = IsPlanarFact;
    types2[1] = HasMinorCompletePartiteFact;
    params2_3[0] = create_function(2);
    params2_3[1] = create_function(3);
    params2_3[2] = create_function(3);
    params2[0] = NULL;
    params2[1] = params2_3;
    param_count2[0] = 0;
    param_count2[1] = 3;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, true);
    // Case 16 - contradiction type 5 should not occur due to params
    types2[0] = IsPlanarFact;
    types2[1] = HasMinorCompletePartiteFact;
    params2_3[0] = create_function(2);
    params2_3[1] = create_function(2);
    params2_3[2] = create_function(3);
    params2[0] = NULL;
    params2[1] = params2_3;
    param_count2[0] = 0;
    param_count2[1] = 3;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 17 - contradiction type 5 should not occur due to types
    types2[0] = IsConnectedFact;
    types2[1] = HasMinorCompletePartiteFact;
    params2_3[0] = create_function(2);
    params2_3[1] = create_function(3);
    params2_3[2] = create_function(3);
    params2[0] = NULL;
    params2[1] = params2_3;
    param_count2[0] = 0;
    param_count2[1] = 3;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 18 - contradiction type 6 should occur
    types2[0] = IsPlanarFact;
    types2[1] = HasMinorCliqueFact;
    params2_1[0] = create_function(5);
    params2[0] = NULL;
    params2[1] = params2_1;
    param_count2[0] = 0;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, true);
    // Case 19 - contradiction type 6 should not occur due to params
    types2[0] = IsPlanarFact;
    types2[1] = HasMinorCliqueFact;
    params2_1[0] = create_function(4);
    params2[0] = NULL;
    params2[1] = params2_1;
    param_count2[0] = 0;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 20 - contradiction type 6 should not occur due to types
    types2[0] = IsConnectedFact;
    types2[1] = HasMinorCliqueFact;
    params2_1[0] = create_function(5);
    params2[0] = NULL;
    params2[1] = params2_1;
    param_count2[0] = 0;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 21 - contradiction type 7 should occur
    types2[0] = HasNoCyclesFact;
    types2[1] = HasCycleFact;
    params2_1[0] = create_function(3);
    params2[0] = NULL;
    params2[1] = params2_1;
    param_count2[0] = 0;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, true);
    // Case 22 - contradiction type 7 should not occur due to types
    types2[0] = IsConnectedFact;
    types2[1] = HasCycleFact;
    params2_1[0] = create_function(3);
    params2[0] = NULL;
    params2[1] = params2_1;
    param_count2[0] = 0;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 23 - contradiction type 8 should occur
    // Case 24 - contradiction type 8 should not occur due to params
    // Case 25 - contradiction type 8 should not occur due to types
    // CONTRADICTION NOT IMPLEMENTED YET
    // Case 26 - contradiction type 9 should occur
    types2[0] = IsPartiteFact;
    types2[1] = HasCycleFact;
    params1_1[0] = create_function(2);
    params2_1[0] = create_function(3);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, true);
    // Case 27 - contradiction type 9 should not occur due to params
    types2[0] = IsPartiteFact;
    types2[1] = HasCycleFact;
    params1_1[0] = create_function(3);
    params2_1[0] = create_function(3);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 28 - contradiction type 9 should not occur due to types
    types2[0] = IsPartiteFact;
    types2[1] = HasPathFact;
    params1_1[0] = create_function(2);
    params2_1[0] = create_function(3);
    params2[0] = params1_1;
    params2[1] = params2_1;
    param_count2[0] = 1;
    param_count2[1] = 1;
    create_and_run_contradict_test_case(types2, params2, param_count2, 2, false);
    // Case 29 - 3 facts case
    types3[0] = IsConnectedFact;
    types3[1] = IsTreeFact;
    types3[2] = IsPlanarFact;
    params3[0] = NULL;
    params3[1] = NULL;
    params3[2] = NULL;
    param_count3[0] = 0;
    param_count3[1] = 0;
    param_count3[2] = 0;
    create_and_run_contradict_test_case(types3, params3, param_count3, 3, false);
}

/**
 * \brief function to run all tests for functions from the contradiction.h file
 */
static void test_contradiction(void)
{
    test_contradict();
}

/**
 * \brief helper to assert 2 facts field by field
 * \param fact1 pointer to first fact
 * \param fact2 pointer to second fact
 */
static void assert_facts(Fact *fact1, Fact *fact2)
{
    if(fact1 == NULL || fact2 == NULL) 
        assert(fact1 == NULL && fact2 == NULL);
    assert(fact1->type == fact2->type);
    assert(fact1->params_count == fact2->params_count);
    for (uint32_t i = 0; i < fact1->params_count; i++)
    {
        assert(compare_functions(fact1->params[i], fact2->params[i]) == 0);
    }
}

static void single_implies_test(Fact **factArray,int fact_count, Fact **expected, int expected_count)
{
    printf("TEST %d\n", counter++);
    // Arrange
    int count;
    // Act
    Fact **result = implies(factArray,fact_count,&count);
    // Assert
    assert(count == expected_count);
    if(expected == NULL)
        assert(result == NULL);
    for(int i=0;i<count;i++)
        assert_facts(expected[i],result[i]);
    // Clean
    delete_fact_array(result,count);
}

static void run_implies_test_case(Implies_Test_Case test_case)
{
    Fact **factArray = create_fact_array_from_modular(test_case.left);
    Fact **expected = create_fact_array_from_modular(test_case.right);
    single_implies_test(factArray,test_case.left.fact_count,expected,test_case.right.fact_count);
    delete_fact_array(factArray,test_case.left.fact_count);
    delete_fact_array(expected,test_case.right.fact_count);
}

static void create_and_run_implies_test_case(FactType *left_types, Function ***left_params, uint8_t *left_params_count, int left_fact_count,
FactType *right_types, Function ***right_params, uint8_t *right_params_count, int right_fact_count)
{
    Implies_Test_Case test_case = {
        .left = {
            .types = left_types,
            .params = left_params,
            .params_count = left_params_count,
            .fact_count = left_fact_count
        },
        .right = {
            .types = right_types,
            .params = right_params,
            .params_count = right_params_count,
            .fact_count = right_fact_count
        }
    };
    run_implies_test_case(test_case);
}

/**
 * \brief function to run all tests for implies method
 */
static void test_implies(void)
{
    FactType left_types1[1]; // use if 1 fact
    FactType left_types2[2]; // use if 2 facts
    FactType left_types3[3]; // use if 3 facts
    FactType right_types1[1]; // use if 1 fact
    FactType right_types2[2]; // use if 2 facts
    Function *left_params1_1[1]; // use for fact 1 if it has 1 param 
    Function *left_params1_2[2]; // use for fact 1 if it has 2 params 
    Function *left_params1_3[3]; // use for fact 1 if it has 3 params 
    Function *left_params2_1[1]; // use for fact 2 if it has 1 param
    Function *left_params2_2[2]; // use for fact 2 if it has 2 params
    Function *left_params2_3[3]; // use for fact 2 if it has 3 params
    Function *left_params3_1[1]; // use for fact 3 if it has 1 param 
    Function *left_params3_2[2]; // use for fact 3 if it has 2 params 
    Function *left_params3_3[3]; // use for fact 3 if it has 3 params
    Function *right_params1_1[1]; // use for fact 1 if it has 1 param 
    Function *right_params1_2[2]; // use for fact 1 if it has 2 params 
    Function *right_params1_3[3]; // use for fact 1 if it has 3 params 
    Function *right_params2_1[1]; // use for fact 2 if it has 1 param
    Function *right_params2_2[2]; // use for fact 2 if it has 2 params
    Function *right_params2_3[3]; // use for fact 2 if it has 3 params

    Function **left_params1[1]; // use if 1 fact
    Function **left_params2[2]; // use if 2 facts
    Function **left_params3[3]; // use if 3 facts
    Function **right_params1[1]; // use if 1 fact
    Function **right_params2[2]; // use if 2 facts
    uint8_t left_param_count1[1];  // use if 1 fact
    uint8_t left_param_count2[2]; // use if 2 facts
    uint8_t left_param_count3[3]; // use if 3 facts
    uint8_t right_param_count1[1];  // use if 1 fact
    uint8_t right_param_count2[2]; // use if 2 facts

    GTD_UNUSED(left_types1);
    GTD_UNUSED(left_types2);
    GTD_UNUSED(left_types3);
    GTD_UNUSED(right_types1);
    GTD_UNUSED(right_types2);
    GTD_UNUSED(left_params1_1);
    GTD_UNUSED(left_params1_2);
    GTD_UNUSED(left_params1_3);
    GTD_UNUSED(left_params2_1);
    GTD_UNUSED(left_params2_2);
    GTD_UNUSED(left_params2_3);
    GTD_UNUSED(left_params3_1);
    GTD_UNUSED(left_params3_2);
    GTD_UNUSED(left_params3_3);
    GTD_UNUSED(right_params1_1);
    GTD_UNUSED(right_params1_2);
    GTD_UNUSED(right_params1_3);
    GTD_UNUSED(right_params2_1);
    GTD_UNUSED(right_params2_2);
    GTD_UNUSED(right_params2_3);
    GTD_UNUSED(left_params1);
    GTD_UNUSED(left_params2);
    GTD_UNUSED(left_params3);
    GTD_UNUSED(right_params1);
    GTD_UNUSED(right_params2);
    GTD_UNUSED(left_param_count1);
    GTD_UNUSED(left_param_count2);
    GTD_UNUSED(left_param_count3);
    GTD_UNUSED(right_param_count1);
    GTD_UNUSED(right_param_count2);
    // Case 1 - empty case
    create_and_run_implies_test_case(NULL,NULL,NULL,0,NULL,NULL,NULL,0);
    // Case 2 - implication type 1
    left_types1[0] = EdgeCountFact;
    left_params1_1[0] = create_function(1);
    left_params1[0] = left_params1_1;
    left_param_count1[0] = 1;
    right_types2[0] = MinEdgeCountFact;
    right_types2[1] = MaxEdgeCountFact; 
    right_params1_1[0] = create_function(1);
    right_params2_1[0] = create_function(1);
    right_params2[0] = right_params1_1;
    right_params2[1] = right_params2_1;
    right_param_count2[0] = 1;
    right_param_count2[1] = 1;
    create_and_run_implies_test_case(left_types1, left_params1, left_param_count1, 1, right_types2, right_params2, right_param_count2, 2);
    // Case 3 - implication type 2
    left_types1[0] = VertexCountFact;
    left_params1_1[0] = create_function(1);
    left_params1[0] = left_params1_1;
    left_param_count1[0] = 1;
    right_types2[0] = MinVertexCountFact;
    right_types2[1] = MaxVertexCountFact; 
    right_params1_1[0] = create_function(1);
    right_params2_1[0] = create_function(1);
    right_params2[0] = right_params1_1;
    right_params2[1] = right_params2_1;
    right_param_count2[0] = 1;
    right_param_count2[1] = 1;
    create_and_run_implies_test_case(left_types1, left_params1, left_param_count1, 1, right_types2, right_params2, right_param_count2, 2);
    // Case 4 - implication type 3
    left_types1[0] = MinEdgeCountFact;
    left_params1_1[0] = create_function(1);
    left_params1[0] = left_params1_1;
    left_param_count1[0] = 1;
    right_types1[0] = MinVertexCountFact;
    right_params1_1[0] = create_function(2);
    right_params1[0] = right_params1_1;
    right_param_count1[0] = 1;
    create_and_run_implies_test_case(left_types1, left_params1, left_param_count1, 1, right_types1, right_params1, right_param_count1, 1);
    // Case 5 - implication type 3 edge case
    left_types1[0] = MinEdgeCountFact;
    left_params1_1[0] = create_function(0);
    left_params1[0] = left_params1_1;
    left_param_count1[0] = 1;
    right_types1[0] = MinVertexCountFact;
    right_params1_1[0] = create_function(0);
    right_params1[0] = right_params1_1;
    right_param_count1[0] = 1;
    create_and_run_implies_test_case(left_types1, left_params1, left_param_count1, 1, right_types1, right_params1, right_param_count1, 1);
    // Case 6 - implication type 4
    left_types1[0] = MaxVertexCountFact;
    left_params1_1[0] = create_function(2);
    left_params1[0] = left_params1_1;
    left_param_count1[0] = 1;
    right_types1[0] = MaxEdgeCountFact;
    right_params1_1[0] = create_function(1);
    right_params1[0] = right_params1_1;
    right_param_count1[0] = 1;
    create_and_run_implies_test_case(left_types1, left_params1, left_param_count1, 1, right_types1, right_params1, right_param_count1, 1);
    // Case 7 - implication type 5
    left_types1[0] = IsTreeFact;
    left_params1[0] = NULL;
    left_param_count1[0] = 0;
    right_types2[0] = IsConnectedFact;
    right_types2[1] = HasNoCyclesFact; 
    right_params2[0] = NULL;
    right_params2[1] = NULL;
    right_param_count2[0] = 0;
    right_param_count2[1] = 0;
    create_and_run_implies_test_case(left_types1, left_params1, left_param_count1, 1, right_types2, right_params2, right_param_count2, 2);
    // Case 8 - implication type 6
    left_types2[0] = IsConnectedFact;
    left_types2[1] = HasNoCyclesFact; 
    left_params2[0] = NULL;
    left_params2[1] = NULL;
    left_param_count2[0] = 0;
    left_param_count2[1] = 0;
    right_types1[0] = IsTreeFact;
    right_params1[0] = NULL;
    right_param_count1[0] = 0;
    create_and_run_implies_test_case(left_types2, left_params2, left_param_count2, 2, right_types1, right_params1, right_param_count1, 1);
    // Case 9 - implication type 7
    // IMPLICATION NOT IMPLEMENTED 
    // Case 10 - implication type 8
    left_types2[0] = IsTreeFact;
    left_types2[1] = HasNoInducedCompletePartiteFact; 
    left_params2_3[0] = create_function(2);
    left_params2_3[1] = create_function(1);
    left_params2_3[2] = create_function(3);
    left_params2[0] = NULL;
    left_params2[1] = left_params2_3;
    left_param_count2[0] = 0;
    left_param_count2[1] = 3;
    right_types1[0] = IstnaryTreeFact;
    right_params1_1[0] = create_function(1);
    right_params1[0] = right_params1_1;
    right_param_count1[0] = 1;
    create_and_run_implies_test_case(left_types2, left_params2, left_param_count2, 2, right_types1, right_params1, right_param_count1, 1);
    // Case 11 - implication type 9
    left_types2[0] = IsTreeFact;
    left_types2[1] = HasNoInducedPathFact; 
    left_params2_1[0] = create_function(2);
    left_params2[0] = NULL;
    left_params2[1] = left_params2_1;
    left_param_count2[0] = 0;
    left_param_count2[1] = 1;
    right_types1[0] = MaxTreeHeightFact;
    right_params1_1[0] = create_function(1);
    right_params1[0] = right_params1_1;
    right_param_count1[0] = 1;
    create_and_run_implies_test_case(left_types2, left_params2, left_param_count2, 2, right_types1, right_params1, right_param_count1, 1);
    // Case - implication type 10
    left_types2[0] = IstnaryTreeFact;
    left_types2[1] = MaxTreeHeightFact; 
    left_params1_1[0] = create_function(2);
    left_params2[0] = left_params1_1;
    left_params2_1[0] = create_function(2);
    left_params2[1] = left_params2_1;
    left_param_count2[0] = 1;
    left_param_count2[1] = 1;
    right_types1[0] = MaxVertexCountFact;
    right_params1_1[0] = create_function(3);
    right_params1[0] = right_params1_1;
    right_param_count1[0] = 1;
    create_and_run_implies_test_case(left_types2, left_params2, left_param_count2, 2, right_types1, right_params1, right_param_count1, 1);
    // Case - implication type 10 edge case
    left_types2[0] = IstnaryTreeFact;
    left_types2[1] = MaxTreeHeightFact; 
    left_params1_1[0] = create_function(1);
    left_params2[0] = left_params1_1;
    left_params2_1[0] = create_function(2);
    left_params2[1] = left_params2_1;
    left_param_count2[0] = 1;
    left_param_count2[1] = 1;
    right_types1[0] = MaxVertexCountFact;
    right_params1_1[0] = create_function(2);
    right_params1[0] = right_params1_1;
    right_param_count1[0] = 1;
    create_and_run_implies_test_case(left_types2, left_params2, left_param_count2, 2, right_types1, right_params1, right_param_count1, 1);
    // Case 13 - impliction type 11
    left_types2[0] = HasNoMinorCompletePartiteFact;
    left_types2[1] = HasNoMinorCliqueFact; 
    left_params1_3[0] = create_function(2);
    left_params1_3[1] = create_function(3);
    left_params1_3[2] = create_function(3);
    left_params2[0] = left_params1_3;
    left_params2_1[0] = create_function(5);
    left_params2[1] = left_params2_1;
    left_param_count2[0] = 3;
    left_param_count2[1] = 1;
    right_types1[0] = IsPlanarFact;
    right_params1[0] = NULL;
    right_param_count1[0] = 0;
    create_and_run_implies_test_case(left_types2, left_params2, left_param_count2, 2, right_types1, right_params1, right_param_count1, 1);
    // Case 14 - impliction type 12
    // IMPLICATION NOT IMPLEMENTED
    // Case 15 - impliction type 13
    // IMPLICATION NOT IMPLEMENTED
    // Case 16 - impliction type 14
    left_types2[0] = IsCycleComplementFact;
    left_types2[1] = MaxVertexCountFact; 
    left_params2[0] = NULL;
    left_params2_1[0] = create_function(6);
    left_params2[1] = left_params2_1;
    left_param_count2[0] = 0;
    left_param_count2[1] = 1;
    right_types1[0] = IsPlanarFact;
    right_params1[0] = NULL;
    right_param_count1[0] = 0;
    create_and_run_implies_test_case(left_types2, left_params2, left_param_count2, 2, right_types1, right_params1, right_param_count1, 1);
    // Case 17 - impliction type 15
    left_types2[0] = IsCycleComplementFact;
    left_types2[1] = VertexCountFact; 
    left_params2[0] = NULL;
    left_params2_1[0] = create_function(7);
    left_params2[1] = left_params2_1;
    left_param_count2[0] = 0;
    left_param_count2[1] = 1;
    right_types1[0] = HasMinorCompletePartiteFact;
    right_params1_3[0] = create_function(2);
    right_params1_3[1] = create_function(3);
    right_params1_3[2] = create_function(3);
    right_params1[0] = right_params1_3;
    right_param_count1[0] = 3;
    create_and_run_implies_test_case(left_types2, left_params2, left_param_count2, 2, right_types1, right_params1, right_param_count1, 1);
    // Case 18 - impliction type 16
    // IMPLICATION NOT IMPLEMENTED
    // Case 19 - impliction type 17
    // IMPLICATION NOT IMPLEMENTED
}

/**
 * \brief function to run all tests for functions from the implication.h file
 */
static void test_implication(void)
{
    test_implies();
}

static void run_function_binary_operation_test_case(Function_Binary_Operation_Test_Case test_case)
{
    printf("TEST %d\n", counter++);
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
    printf("TEST %d\n", counter++);
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
    printf("TEST %d\n", counter++);
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
    printf("TEST %d\n", counter++);
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
    printf("TEST %d\n", counter++);
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
    printf("TEST %d\n", counter++);
    // Act
    uint8_t ret = get_param_count(type);
    // Assert
    assert(ret == expected);
}

static void run_get_fact_str_test(Fact *fact, char expected[MAX_FACT_STR_LEN])
{
    printf("TEST %d\n", counter++);
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
    Fact *fact = create_fact(type, params, param_count);
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
    Fact *fact = create_fact(type, params, param_count);
    gtd_free(params);
    return fact;
}

static void run_equal_facts_test(Fact *fact1, Fact *fact2, bool expected)
{
    printf("TEST %d\n", counter++);
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

/**
 * \brief function to run all required tests for gtd_fact_tree module
 */
void test_fact_tree(void)
{
    test_function();
    test_fact();
    test_contradiction();
    test_implication();
}