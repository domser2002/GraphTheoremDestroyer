#include "test_fact_tree.h"

typedef Fact *(*create_fact_func)(int);
typedef int (*delete_fact_func)(Fact *);

/**
 * \brief helper function to perform a single test on a contradict method
 * \param n_facts number of facts
 * \param create_facts array of pointers to functions used to create facts
 * \param delete_facts array of pointers to functions used to delete facts
 * \param fact_params array of parameters used to create facts
 * \param expected expected contradict return value
 */
static void single_contradict_test(int n_facts, create_fact_func *create_facts, delete_fact_func *delete_facts, int *fact_params, bool expected)
{
    // Arrange
    Fact **factArray = (Fact **)gtd_malloc(n_facts * sizeof(Fact *));
    for (int i = 0; i < n_facts; i++)
    {
        factArray[i] = create_facts[i](fact_params[i]);
    }
    // Act
    bool result = contradict(factArray, n_facts);
    // Clean
    for (int i = 0; i < n_facts; i++)
    {
        delete_facts[i](factArray[i]);
    }
    gtd_free(factArray);
    // Assert
    assert(result == expected);
}

/**
* \brief function to check if contradict correctly recognizes contradiction type 1 -
contradiction between max vertex count and min edge count
* \param maxVertexCount max vertex count
* \param minEdgeCount min edge count
* \param expected expected test result, true if facts contradict
*/
static void test_contradiction_type_1(int maxVertexCount, int minEdgeCount, bool expected)
{
    int n_facts = 2;
    create_fact_func *create_fact_functions = (create_fact_func *)gtd_malloc(n_facts * sizeof(create_fact_func));
    delete_fact_func *delete_fact_functions = (delete_fact_func *)gtd_malloc(n_facts * sizeof(delete_fact_func));
    int *fact_params = (int *)gtd_malloc(n_facts * sizeof(int));
    create_fact_functions[0] = &create_max_vertex_count_fact;
    delete_fact_functions[0] = &delete_max_vertex_count_fact;
    fact_params[0] = maxVertexCount;
    create_fact_functions[1] = &create_min_edge_count_fact;
    delete_fact_functions[1] = &delete_min_edge_count_fact;
    fact_params[1] = minEdgeCount;
    single_contradict_test(n_facts, create_fact_functions, delete_fact_functions, fact_params, expected);
    gtd_free(create_fact_functions);
    gtd_free(delete_fact_functions);
    gtd_free(fact_params);
}

/**
* \brief function to check if contradict correctly recognizes contradiction type 2 -
contradiction between min vertex count and max vertex count
* \param minVertexCount min vertex count
* \param maxVertexCount max vertex count
* \param expected expected test result, true if facts contradict
*/
static void test_contradiction_type_2(int minVertexCount, int maxVertexCount, bool expected)
{
    int n_facts = 2;
    create_fact_func *create_fact_functions = (create_fact_func *)gtd_malloc(n_facts * sizeof(create_fact_func));
    delete_fact_func *delete_fact_functions = (delete_fact_func *)gtd_malloc(n_facts * sizeof(delete_fact_func));
    int *fact_params = (int *)gtd_malloc(n_facts * sizeof(int));
    create_fact_functions[0] = &create_min_vertex_count_fact;
    delete_fact_functions[0] = &delete_min_vertex_count_fact;
    fact_params[0] = minVertexCount;
    create_fact_functions[1] = &create_max_vertex_count_fact;
    delete_fact_functions[1] = &delete_max_vertex_count_fact;
    fact_params[1] = maxVertexCount;
    single_contradict_test(n_facts, create_fact_functions, delete_fact_functions, fact_params, expected);
    gtd_free(create_fact_functions);
    gtd_free(delete_fact_functions);
    gtd_free(fact_params);
}

/**
* \brief function to check if contradict correctly recognizes contradiction type 3 -
contradiction between min edge count and max edge count
* \param minEdgeCount min edge count
* \param maxEdgeCount max edge count
* \param expected expected test result, true if facts contradict
*/
static void test_contradiction_type_3(int minEdgeCount, int maxEdgeCount, bool expected)
{
    int n_facts = 2;
    create_fact_func *create_fact_functions = (create_fact_func *)gtd_malloc(n_facts * sizeof(create_fact_func));
    delete_fact_func *delete_fact_functions = (delete_fact_func *)gtd_malloc(n_facts * sizeof(delete_fact_func));
    int *fact_params = (int *)gtd_malloc(n_facts * sizeof(int));
    create_fact_functions[0] = &create_min_edge_count_fact;
    delete_fact_functions[0] = &delete_min_edge_count_fact;
    fact_params[0] = minEdgeCount;
    create_fact_functions[1] = &create_max_edge_count_fact;
    delete_fact_functions[1] = &delete_max_edge_count_fact;
    fact_params[1] = maxEdgeCount;
    single_contradict_test(n_facts, create_fact_functions, delete_fact_functions, fact_params, expected);
    gtd_free(create_fact_functions);
    gtd_free(delete_fact_functions);
    gtd_free(fact_params);
}

/**
 * \brief function to run all tests for contradict method
 */
static void test_contradict(void)
{
    test_contradiction_type_1(1, 1, true);
    test_contradiction_type_1(1, 0, false);
    test_contradiction_type_1(0, 0, false);
    test_contradiction_type_1(10000, 10000 * 9999 / 2 + 1, true);
    test_contradiction_type_1(10000, 10000 * 9999 / 2, false);
    test_contradiction_type_2(0, 0, false);
    test_contradiction_type_2(1, 0, true);
    test_contradiction_type_2(0, 1, false);
    test_contradiction_type_2(1234567, 1234566, true);
    test_contradiction_type_3(0, 0, false);
    test_contradiction_type_3(1, 0, true);
    test_contradiction_type_3(0, 1, false);
    test_contradiction_type_3(1234567, 1234566, true);
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

/**
 * \brief helper function to perform a single test on the implies method
 * \param n_facts_left number of left side facts
 * \param create_facts_left array of pointers to functions used to create left side facts
 * \param delete_facts_left array of pointers to functions used to delete left side facts
 * \param fact_params_left array of parameters used to create left side facts
 * \param n_facts_right expected number of right side facts
 * \param create_facts_right array of pointers to functions used to create expected right side facts
 * \param delete_facts_right array of pointers to functions used to delete expected right side facts
 * \param fact_params_right array of parameters used to create expected right side facts
 */
static void single_implies_test(int n_facts_left, create_fact_func *create_facts_left,
                                delete_fact_func *delete_facts_left, int *fact_params_left,
                                int n_facts_right, create_fact_func *create_facts_right,
                                delete_fact_func *delete_facts_right, int *fact_params_right)
{
    // Arrange
    Fact **factArray_left = (Fact **)gtd_malloc(n_facts_left * sizeof(Fact *));
    for (int i = 0; i < n_facts_left; i++)
    {
        factArray_left[i] = create_facts_left[i](fact_params_left[i]);
    }
    Fact **factArray_right = (Fact **)gtd_malloc(n_facts_right * sizeof(Fact *));
    for (int i = 0; i < n_facts_right; i++)
    {
        factArray_right[i] = create_facts_right[i](fact_params_right[i]);
    }
    int count = 0;
    // Act
    Fact **result = implies(factArray_left, n_facts_left, &count);
    // Clean left
    for (int i = 0; i < n_facts_left; i++)
    {
        delete_facts_left[i](factArray_left[i]);
    }
    gtd_free(factArray_left);
    // Assert
    assert(count == n_facts_right);
    for (int i = 0; i < count; i++)
    {
        assert_facts(result[i], factArray_right[i]);
    }
    // Clean right
    for (int i = 0; i < n_facts_right; i++)
    {
        delete_facts_right[i](factArray_right[i]);
        delete_facts_right[i](result[i]);
    }
    gtd_free(factArray_right);
    gtd_free(result);
}

static void test_implication_type_1(int minEdgeCount, int minVertexCount)
{
    int n_facts_left = 1, n_facts_right = 1;
    create_fact_func *create_fact_left = (create_fact_func *)gtd_malloc(n_facts_left * sizeof(create_fact_func));
    create_fact_func *create_fact_right = (create_fact_func *)gtd_malloc(n_facts_right * sizeof(create_fact_func));
    delete_fact_func *delete_fact_left = (delete_fact_func *)gtd_malloc(n_facts_left * sizeof(delete_fact_func));
    delete_fact_func *delete_fact_right = (delete_fact_func *)gtd_malloc(n_facts_right * sizeof(delete_fact_func));
    int *fact_params_left = (int *)gtd_malloc(n_facts_left * sizeof(int));
    int *fact_params_right = (int *)gtd_malloc(n_facts_right * sizeof(int));
    create_fact_left[0] = &create_min_edge_count_fact;
    create_fact_right[0] = &create_min_vertex_count_fact;
    delete_fact_left[0] = &delete_min_edge_count_fact;
    delete_fact_right[0] = &delete_min_vertex_count_fact;
    fact_params_left[0] = minEdgeCount;
    fact_params_right[0] = minVertexCount;
    single_implies_test(n_facts_left, create_fact_left, delete_fact_left, fact_params_left, n_facts_right, create_fact_right, delete_fact_right, fact_params_right);
    gtd_free(create_fact_left);
    gtd_free(create_fact_right);
    gtd_free(delete_fact_left);
    gtd_free(delete_fact_right);
    gtd_free(fact_params_left);
    gtd_free(fact_params_right);
}

static void test_implication_type_2(int maxVertexCount, int maxEdgeCount)
{
    int n_facts_left = 1, n_facts_right = 1;
    create_fact_func *create_fact_left = (create_fact_func *)gtd_malloc(n_facts_left * sizeof(create_fact_func));
    create_fact_func *create_fact_right = (create_fact_func *)gtd_malloc(n_facts_right * sizeof(create_fact_func));
    delete_fact_func *delete_fact_left = (delete_fact_func *)gtd_malloc(n_facts_left * sizeof(delete_fact_func));
    delete_fact_func *delete_fact_right = (delete_fact_func *)gtd_malloc(n_facts_right * sizeof(delete_fact_func));
    int *fact_params_left = (int *)gtd_malloc(n_facts_left * sizeof(int));
    int *fact_params_right = (int *)gtd_malloc(n_facts_right * sizeof(int));
    create_fact_left[0] = &create_max_vertex_count_fact;
    create_fact_right[0] = &create_max_edge_count_fact;
    delete_fact_left[0] = &delete_max_vertex_count_fact;
    delete_fact_right[0] = &delete_max_edge_count_fact;
    fact_params_left[0] = maxVertexCount;
    fact_params_right[0] = maxEdgeCount;
    single_implies_test(n_facts_left, create_fact_left, delete_fact_left, fact_params_left, n_facts_right, create_fact_right, delete_fact_right, fact_params_right);
    gtd_free(create_fact_left);
    gtd_free(create_fact_right);
    gtd_free(delete_fact_left);
    gtd_free(delete_fact_right);
    gtd_free(fact_params_left);
    gtd_free(fact_params_right);
}

/**
 * \brief function to run all tests for implies method
 */
static void test_implies(void)
{
    test_implication_type_1(0,0);
    test_implication_type_1(1,2);
    test_implication_type_1(3,3);
    test_implication_type_1(1000,46);
    test_implication_type_2(0,0);
    test_implication_type_2(1,0);
    test_implication_type_2(2,1);
    test_implication_type_2(1000,1000 * 999 / 2);
}

/**
 * \brief function to run all tests for functions from the implication.h file
 */
static void test_implication(void)
{
    test_implies();
}

/**
 * \brief function to run all required tests for files from gtd_fact_tree
 */
void test_fact_tree(void)
{
    test_contradiction();
    test_implication();
}