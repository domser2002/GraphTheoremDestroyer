#define IS_FACT_TREE_COMPONENT
#include "test_gtd_fact_tree.h"
static uint8_t unit_test_counter = 1;
static uint8_t integration_test_counter = 1;
#define FACT_TREE_UNIT_TEST_NUM 38
#define FACT_TREE_INTEGRATION_TEST_NUM 7

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

static void write_unit_test_num(void)
{
    printf("\rTEST %d/%d", unit_test_counter++, FACT_TREE_UNIT_TEST_NUM);
    fflush(stdout);
}

static void write_integration_test_num(void)
{
    printf("\rTEST %d/%d", integration_test_counter++, FACT_TREE_INTEGRATION_TEST_NUM);
    fflush(stdout);
}

static Fact **create_fact_array_from_modular(Modular_Fact_Array mfa)
{
    Fact **factArray = (Fact**)gtd_malloc(mfa.fact_count * sizeof(Fact*));
    for(uint32_t i=0;i<mfa.fact_count;i++)
        factArray[i] = create_fact(mfa.types[i],mfa.params[i]);
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
    write_unit_test_num();
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
    write_unit_test_num();
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

/**
 * \brief function to run all required tests for gtd_fact_tree module
 */
void gtd_fact_tree_unit_tests(void)
{
    printf("RUN UNIT TESTS\n");
    test_contradiction();
    test_implication();
}

typedef struct FactTreeMachineTestCase {
    uint32_t fact_count;
    Fact **fact_array;
    bool expected;
} FactTreeMachineTestCase;

static void run_fact_tree_machine_test_case(FactTreeMachineTestCase test_case)
{
    write_integration_test_num();
    // Arrange 
    FactTree *ft = construct(test_case.fact_count, &test_case.fact_array);
    FactTreeMachine *machine = init_machine(ft);
    // Act
    execute(machine);
    // Assert
    assert(get_contradiction_found(machine) == test_case.expected);
    // Clean
    delete_machine(machine);
    destruct(ft);
}

static void test_fact_tree_machine(void)
{
    Function *params1[1];
    Function *params3[3];
    FactTreeMachineTestCase test_case;
    test_case.fact_count = 0;
    test_case.fact_array = NULL;
    test_case.expected = false;
    run_fact_tree_machine_test_case(test_case);

    test_case.fact_count = 1;
    test_case.fact_array = (Fact **)gtd_malloc(1 * sizeof(Fact*));
    test_case.fact_array[0] = create_fact(IsConnectedFact, NULL);
    test_case.expected = false;
    run_fact_tree_machine_test_case(test_case);
    delete_fact(test_case.fact_array[0]);
    gtd_free(test_case.fact_array);

    test_case.fact_count = 2;
    test_case.fact_array = (Fact **)gtd_malloc(2 * sizeof(Fact*));
    test_case.fact_array[0] = create_fact(HasNoCyclesFact, NULL);
    params1[0] = create_function(3);
    test_case.fact_array[1] = create_fact(HasCycleFact, params1);
    test_case.expected = true;
    run_fact_tree_machine_test_case(test_case);
    delete_fact(test_case.fact_array[0]);
    delete_fact(test_case.fact_array[1]);
    gtd_free(test_case.fact_array);

    test_case.fact_count = 3;
    test_case.fact_array = (Fact **)gtd_malloc(3 * sizeof(Fact*));
    test_case.fact_array[0] = create_fact(HasNoCyclesFact, NULL);
    params1[0] = create_function(3);
    test_case.fact_array[1] = create_fact(HasCycleFact, params1);
    test_case.fact_array[2] = create_fact(IsConnectedFact, NULL);
    test_case.expected = true;
    run_fact_tree_machine_test_case(test_case);
    delete_fact(test_case.fact_array[0]);
    delete_fact(test_case.fact_array[1]);
    delete_fact(test_case.fact_array[2]);
    gtd_free(test_case.fact_array);

    test_case.fact_count = 2;
    test_case.fact_array = (Fact **)gtd_malloc(2 * sizeof(Fact*));
    test_case.fact_array[0] = create_fact(IsTreeFact, NULL);
    params1[0] = create_function(3);
    test_case.fact_array[1] = create_fact(HasCycleFact, params1);
    test_case.expected = true;
    run_fact_tree_machine_test_case(test_case);

    test_case.fact_count = 5;
    test_case.fact_array = (Fact **)gtd_malloc(5 * sizeof(Fact*));
    test_case.fact_array[0] = create_fact(HasNoCyclesFact, NULL);
    params3[0] = create_function(2);
    params3[1] = create_function(1);
    params3[2] = create_function(4);
    test_case.fact_array[1] = create_fact(HasNoInducedCompletePartiteFact, params3);
    params1[0] = create_function(4);
    test_case.fact_array[2] = create_fact(HasNoInducedPathFact, params1);
    params1[0] = create_function(8);
    test_case.fact_array[3] = create_fact(MinVertexCountFact, params1);
    test_case.fact_array[4] = create_fact(IsConnectedFact, NULL);
    test_case.expected = true;
    run_fact_tree_machine_test_case(test_case);

    test_case.fact_count = 10;
    test_case.fact_array = (Fact **)gtd_malloc(10 * sizeof(Fact*));
    test_case.fact_array[0] = create_fact(HasNoCyclesFact, NULL);
    params3[0] = create_function(2);
    params3[1] = create_function(1);
    params3[2] = create_function(4);
    test_case.fact_array[1] = create_fact(HasNoInducedCompletePartiteFact, params3);
    params1[0] = create_function(4);
    test_case.fact_array[2] = create_fact(HasNoInducedPathFact, params1);
    params1[0] = create_function(8);
    test_case.fact_array[3] = create_fact(MinVertexCountFact, params1);
    test_case.fact_array[4] = create_fact(IsConnectedFact, NULL);
    params1[0] = create_function(8);
    test_case.fact_array[5] = create_fact(HasNoInducedCycleFact, params1);
    params1[0] = create_function(8);
    test_case.fact_array[6] = create_fact(HasNoMinorCycleFact, params1);
    params1[0] = create_function(8);
    test_case.fact_array[7] = create_fact(HasNoCliqueFact, params1);
    params1[0] = create_function(8);
    test_case.fact_array[8] = create_fact(HasNoMinorCliqueFact, params1);
    params1[0] = create_function(8);
    test_case.fact_array[9] = create_fact(MaxVertexDegreeFact, params1);
    test_case.expected = true;
    run_fact_tree_machine_test_case(test_case);
}

void gtd_fact_tree_integration_tests(void)
{
    printf("\nRUN INTEGRATION TESTS\n");
    test_fact_tree_machine();
}