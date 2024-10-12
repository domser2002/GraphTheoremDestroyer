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
    Fact **factArray = (Fact **)gtd_malloc(n_facts*sizeof(Fact*));
    for(int i=0; i<n_facts; i++)
    {
        factArray[i] = create_facts[i](fact_params[i]);
    }
    // Act
    bool result = contradict(factArray,n_facts);
    // Clean
    for(int i=0; i<n_facts; i++)
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
static void test_contradiction_1(int maxVertexCount, int minEdgeCount, bool expected)
{
    int n_facts = 2;
    create_fact_func *create_fact_functions = (create_fact_func*)gtd_malloc(n_facts*sizeof(create_fact_func));
    delete_fact_func *delete_fact_functions = (delete_fact_func*)gtd_malloc(n_facts*sizeof(delete_fact_func));
    int *fact_params = (int*)gtd_malloc(n_facts*sizeof(int));
    create_fact_functions[0] = &create_max_vertex_count_fact;
    delete_fact_functions[0] = &delete_max_vertex_count_fact;
    fact_params[0] = maxVertexCount;
    create_fact_functions[1] = &create_min_edge_count_fact;
    delete_fact_functions[1] = &delete_min_edge_count_fact;
    fact_params[1] = minEdgeCount;
    single_contradict_test(n_facts,create_fact_functions,delete_fact_functions,fact_params,expected);
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
static void test_contradiction_2(int minVertexCount, int maxVertexCount, bool expected)
{
    int n_facts = 2;
    create_fact_func *create_fact_functions = (create_fact_func*)gtd_malloc(n_facts*sizeof(create_fact_func));
    delete_fact_func *delete_fact_functions = (delete_fact_func*)gtd_malloc(n_facts*sizeof(delete_fact_func));
    int *fact_params = (int*)gtd_malloc(n_facts*sizeof(int));
    create_fact_functions[0] = &create_min_vertex_count_fact;
    delete_fact_functions[0] = &delete_min_vertex_count_fact;
    fact_params[0] = minVertexCount;
    create_fact_functions[1] = &create_max_vertex_count_fact;
    delete_fact_functions[1] = &delete_max_vertex_count_fact;
    fact_params[1] = maxVertexCount;
    single_contradict_test(n_facts,create_fact_functions,delete_fact_functions,fact_params,expected);
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
static void test_contradiction_3(int minEdgeCount, int maxEdgeCount, bool expected)
{
    int n_facts = 2;
    create_fact_func *create_fact_functions = (create_fact_func*)gtd_malloc(n_facts*sizeof(create_fact_func));
    delete_fact_func *delete_fact_functions = (delete_fact_func*)gtd_malloc(n_facts*sizeof(delete_fact_func));
    int *fact_params = (int*)gtd_malloc(n_facts*sizeof(int));
    create_fact_functions[0] = &create_min_edge_count_fact;
    delete_fact_functions[0] = &delete_min_edge_count_fact;
    fact_params[0] = minEdgeCount;
    create_fact_functions[1] = &create_max_edge_count_fact;
    delete_fact_functions[1] = &delete_max_edge_count_fact;
    fact_params[1] = maxEdgeCount;
    single_contradict_test(n_facts,create_fact_functions,delete_fact_functions,fact_params,expected);
    gtd_free(create_fact_functions);
    gtd_free(delete_fact_functions);
    gtd_free(fact_params);
}

/**
 * \brief function to run all tests for the contradict method
*/
static void test_contradict(void)
{
    test_contradiction_1(1,1,true);
    test_contradiction_1(1,0,false);
    test_contradiction_1(0,0,false);
    test_contradiction_1(10000,10000 * 9999 / 2 + 1,true);
    test_contradiction_1(10000,10000 * 9999 / 2,false);
    test_contradiction_2(0,0,false);
    test_contradiction_2(1,0,true);
    test_contradiction_2(0,1,false);
    test_contradiction_2(1234567,1234566,true);
    test_contradiction_3(0,0,false);
    test_contradiction_3(1,0,true);
    test_contradiction_3(0,1,false);
    test_contradiction_3(1234567,1234566,true);
}

static void test_results(void)
{
}

/**
 * \brief function to run all required tests for functions from fact.h
*/
void test_fact(void)
{
    test_contradict();
    test_results();
}