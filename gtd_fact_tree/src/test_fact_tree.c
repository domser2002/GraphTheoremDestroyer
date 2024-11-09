#include "test_fact_tree.h"

typedef Fact *(*create_fact_func)(int);
typedef int (*delete_fact_func)(Fact *);

typedef struct Modular_Fact_Array {
    FactType *types;
    int **params;
    int *params_count;
    int fact_count;
} Modular_Fact_Array;

typedef struct Implies_Test_Case {
    Modular_Fact_Array left;
    Modular_Fact_Array right;
} Implies_Test_Case;

typedef struct Contradict_Test_Case {
    Modular_Fact_Array facts;
    bool expected;
} Contradict_Test_Case;

static Fact **create_fact_array_from_modular(Modular_Fact_Array mfa)
{
    Fact **factArray = (Fact**)gtd_malloc(mfa.fact_count * sizeof(Fact*));
    for(int i=0;i<mfa.fact_count;i++)
        factArray[i] = create_fact(mfa.types[i],mfa.params[i],mfa.params_count[i]);
    return factArray;
}

static void delete_fact_array(Fact **factArray,int fact_count)
{
    for(int i=0;i<fact_count;i++)
        delete_fact(factArray[i]);
    gtd_free(factArray);
}

static void single_contradict_test(Fact **factArray, int fact_count, bool expected)
{
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

static void create_and_run_contradict_test_case(FactType *types, int **params, int *params_count, int fact_count, bool expected)
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
    // Case 1 - no facts case
    create_and_run_contradict_test_case(NULL,NULL,NULL,0,false);
    // Case 2 - only MinVertexCountFact case
    FactType types2[] = {MinVertexCountFact};
    int params2_1[] = {1};
    int *params2[] = {params2_1};
    int params_count_2[] = {1};
    int fact_count_2 = 1;
    create_and_run_contradict_test_case(types2,params2,params_count_2,fact_count_2, false);
    // Case 3 - only MaxVertexCountFact case
    FactType types3[] = {MaxVertexCountFact};
    int params3_1[] = {1};
    int *params3[] = {params3_1};
    int params_count_3[] = {1};
    int fact_count_3 = 1;
    create_and_run_contradict_test_case(types3,params3,params_count_3,fact_count_3, false);
    // Case 4 - only MinEdgeCountFact case
    FactType types4[] = {MinEdgeCountFact};
    int params4_1[] = {1};
    int *params4[] = {params4_1};
    int params_count_4[] = {1};
    int fact_count_4 = 1;
    create_and_run_contradict_test_case(types4,params4,params_count_4,fact_count_4, false);
    // Case 5 - only MaxVertexCountFact case
    FactType types5[] = {MaxVertexCountFact};
    int params5_1[] = {1};
    int *params5[] = {params5_1};
    int params_count_5[] = {1};
    int fact_count_5 = 1;
    create_and_run_contradict_test_case(types5,params5,params_count_5,fact_count_5, false);
    // Case 6 - contradiction type 1 should not occur
    FactType types6[] = {MaxVertexCountFact, MinEdgeCountFact};
    int params6_1[] = {0};
    int params6_2[] = {0};
    int *params6[] = {params6_1, params6_2};
    int params_count_6[] = {1,1};
    int fact_count_6 = 2;
    create_and_run_contradict_test_case(types6,params6,params_count_6,fact_count_6, false);
    // Case 7 - contradiction type 1 should occur
    FactType types7[] = {MaxVertexCountFact, MinEdgeCountFact};
    int params7_1[] = {1};
    int params7_2[] = {1};
    int *params7[] = {params7_1, params7_2};
    int params_count_7[] = {1,1};
    int fact_count_7 = 2;
    create_and_run_contradict_test_case(types7,params7,params_count_7,fact_count_7, true);
    // Case 8 - contradiction type 2 should not occur
    FactType types8[] = {MinVertexCountFact, MaxVertexCountFact};
    int params8_1[] = {1};
    int params8_2[] = {1};
    int *params8[] = {params8_1, params8_2};
    int params_count_8[] = {1,1};
    int fact_count_8 = 2;
    create_and_run_contradict_test_case(types8,params8,params_count_8,fact_count_8, false);
    // Case 9 - contradiction type 2 should occur
    FactType types9[] = {MinVertexCountFact, MaxVertexCountFact};
    int params9_1[] = {2};
    int params9_2[] = {1};
    int *params9[] = {params9_1, params9_2};
    int params_count_9[] = {1,1};
    int fact_count_9 = 2;
    create_and_run_contradict_test_case(types9,params9,params_count_9,fact_count_9, true);
    // Case 10 - contradiction type 3 should not occur
    FactType types10[] = {MinEdgeCountFact, MaxEdgeCountFact};
    int params10_1[] = {1};
    int params10_2[] = {1};
    int *params10[] = {params10_1, params10_2};
    int params_count_10[] = {1,1};
    int fact_count_10 = 2;
    create_and_run_contradict_test_case(types10,params10,params_count_10,fact_count_10, false);
    // Case 11 - contradiction type 3 should occur
    FactType types11[] = {MinEdgeCountFact, MaxEdgeCountFact};
    int params11_1[] = {2};
    int params11_2[] = {1};
    int *params11[] = {params11_1, params11_2};
    int params_count_11[] = {1,1};
    int fact_count_11 = 2;
    create_and_run_contradict_test_case(types11,params11,params_count_11,fact_count_11, true);
    // Case 12 - contradiction type 1 additional fact
    FactType types12[] = {MaxVertexCountFact, MinEdgeCountFact, MinVertexCountFact};
    int params12_1[] = {1};
    int params12_2[] = {1};
    int params12_3[] = {1};
    int *params12[] = {params12_1, params12_2, params12_3};
    int params_count_12[] = {1,1,1};
    int fact_count_12 = 3;
    create_and_run_contradict_test_case(types12,params12,params_count_12,fact_count_12, false); 
    // Case 13 - contradiction type 2 additional fact 
    FactType types13[] = {MinVertexCountFact, MaxVertexCountFact, MinEdgeCountFact};
    int params13_1[] = {2};
    int params13_2[] = {1};
    int params13_3[] = {1};
    int *params13[] = {params13_1, params13_2, params13_3};
    int params_count_13[] = {1,1,1};
    int fact_count_13 = 3;
    create_and_run_contradict_test_case(types13,params13,params_count_13,fact_count_13, false);
    // Case 14 - contradiction type 3 additional fact 
    FactType types14[] = {MinEdgeCountFact, MaxEdgeCountFact, MinVertexCountFact};
    int params14_1[] = {2};
    int params14_2[] = {1};
    int params14_3[] = {1};
    int *params14[] = {params14_1, params14_2, params14_3};
    int params_count_14[] = {1,1,1};
    int fact_count_14 = 3;
    create_and_run_contradict_test_case(types14,params14,params_count_14,fact_count_14, false);
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
    assert(fact1->type == fact2->type);
    assert(fact1->params_count == fact2->params_count);
    for (int i = 0; i < fact1->params_count; i++)
    {
        assert(fact1->params[i] == fact2->params[i]);
    }
}

static void single_implies_test(Fact **factArray,int fact_count, Fact **expected, int expected_count)
{
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

static void create_and_run_implies_test_case(FactType *left_types, int **left_params, int *left_params_count, int left_fact_count,
FactType *right_types, int **right_params, int *right_params_count, int right_fact_count)
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
    // Case 1 - empty case
    create_and_run_implies_test_case(NULL,NULL,NULL,0,NULL,NULL,NULL,0);
    // Case 2 - edge case for implication type 1
    // left side
    FactType left2_types[] = {MinEdgeCountFact};
    int left2_params1[] = {0};
    int *left2_params[] = {left2_params1};
    int left2_params_count[] = {1};
    int left2_fact_count = 1;
    // expected right side
    FactType right2_types[] = {MinVertexCountFact};
    int right2_params1[] = {0};
    int *right2_params[] = {right2_params1};
    int right2_params_count[] = {1};
    int right2_fact_count = 1;
    create_and_run_implies_test_case(left2_types,left2_params,left2_params_count,left2_fact_count,right2_types,right2_params,right2_params_count,right2_fact_count);
    // Case 3 - normal case for implication type 1
    // left side
    FactType left3_types[] = {MinEdgeCountFact};
    int left3_params1[] = {1};
    int *left3_params[] = {left3_params1};
    int left3_params_count[] = {1};
    int left3_fact_count = 1;
    // expected right side
    FactType right3_types[] = {MinVertexCountFact};
    int right3_params1[] = {2};
    int *right3_params[] = {right3_params1};
    int right3_params_count[] = {1};
    int right3_fact_count = 1;
    create_and_run_implies_test_case(left3_types,left3_params,left3_params_count,left3_fact_count,
    right3_types,right3_params,right3_params_count,right3_fact_count);
    // Case 4 - edge case for implication type 2
    // left side
    FactType left4_types[] = {MaxVertexCountFact};
    int left4_params1[] = {0};
    int *left4_params[] = {left4_params1};
    int left4_params_count[] = {1};
    int left4_fact_count = 1;
    // expected right side
    FactType right4_types[] = {MaxEdgeCountFact};
    int right4_params1[] = {0};
    int *right4_params[] = {right4_params1};
    int right4_params_count[] = {1};
    int right4_fact_count = 1;
    create_and_run_implies_test_case(left4_types,left4_params,left4_params_count,left4_fact_count,right4_types,right4_params,right4_params_count,right4_fact_count);
    // Case 5 - normal case for implication type 2
    // left side
    FactType left5_types[] = {MaxVertexCountFact};
    int left5_params1[] = {2};
    int *left5_params[] = {left5_params1};
    int left5_params_count[] = {1};
    int left5_fact_count = 1;
    // expected right side
    FactType right5_types[] = {MaxEdgeCountFact};
    int right5_params1[] = {1};
    int *right5_params[] = {right5_params1};
    int right5_params_count[] = {1};
    int right5_fact_count = 1;
    create_and_run_implies_test_case(left5_types,left5_params,left5_params_count,left5_fact_count,right5_types,right5_params,right5_params_count,right5_fact_count);
    // Case 6 - MaxEdgeCountFact implies nothing
    // left side
    FactType left6_types[] = {MaxEdgeCountFact};
    int left6_params1[] = {1};
    int *left6_params[] = {left6_params1};
    int left6_params_count[] = {1};
    int left6_fact_count = 1;
    create_and_run_implies_test_case(left6_types,left6_params,left6_params_count,left6_fact_count,NULL,NULL,NULL,0);
    // Case 7 - MinVertexCountFact implies nothing
    // left side
    FactType left7_types[] = {MinVertexCountFact};
    int left7_params1[] = {1};
    int *left7_params[] = {left7_params1};
    int left7_params_count[] = {1};
    int left7_fact_count = 1;
    create_and_run_implies_test_case(left7_types,left7_params,left7_params_count,left7_fact_count,NULL,NULL,NULL,0);
    // Case 8 - additional fact to break implication type 1
    // left side
    FactType left8_types[] = {MinEdgeCountFact,MaxVertexCountFact};
    int left8_params1[] = {1};
    int left8_params2[] = {1};
    int *left8_params[] = {left8_params1, left8_params2};
    int left8_params_count[] = {1,1};
    int left8_fact_count = 2;
    create_and_run_implies_test_case(left8_types,left8_params,left8_params_count,left8_fact_count,NULL,NULL,NULL,0);
    // Case 9 - additional fact to break implication type 2
    // left side
    FactType left9_types[] = {MaxVertexCountFact,MaxEdgeCountFact};
    int left9_params1[] = {1};
    int left9_params2[] = {1};
    int *left9_params[] = {left9_params1, left9_params2};
    int left9_params_count[] = {1,1};
    int left9_fact_count = 2;
    create_and_run_implies_test_case(left9_types,left9_params,left9_params_count,left9_fact_count,NULL,NULL,NULL,0);
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
void test_fact_tree(void)
{
    test_contradiction();
    test_implication();
}