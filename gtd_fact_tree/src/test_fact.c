#include "test_fact.h"

typedef Fact *(*create_fact_func)(int);
typedef int (*delete_fact_func)(Fact *);

static void single_contradict_test(create_fact_func create_fact1, int fact1_param, create_fact_func create_fact2, int fact2_param, delete_fact_func delete_fact1, delete_fact_func delete_fact2, bool expected)
{
    // Arrange
    Fact **factArray = (Fact **)gtd_malloc(2*sizeof(Fact*));
    factArray[0] = create_fact1(fact1_param);
    factArray[1] = create_fact2(fact2_param);
    // Act
    bool result = contradict(factArray,2);
    // Clean
    delete_fact1(factArray[0]);
    delete_fact2(factArray[1]);
    gtd_free(factArray);
    // Assert
    assert(result == expected);
}

/**
* \brief function to check if contradiction correctly recognizes contradiction type 1 - 
contradiction between max vertex count and min edge count
* \param maxVertexCount max vertex count
* \param minEdgeCount min edge count
* \param expected expected test result, true if facts contradict
*/
static void test_contradiction_1(int maxVertexCount, int minEdgeCount, bool expected)
{
    single_contradict_test(&create_max_vertex_count_fact, maxVertexCount, 
    &create_min_edge_count_fact, minEdgeCount,
    &delete_max_vertex_count_fact, &delete_min_edge_count_fact, expected);
}

static void test_contradiction_2(int minVertexCount, int maxVertexCount, bool expected)
{
    single_contradict_test(&create_min_vertex_count_fact, minVertexCount, 
    &create_max_vertex_count_fact, maxVertexCount,
    &delete_min_vertex_count_fact, &delete_max_vertex_count_fact, expected);
}

static void test_contradiction_3(int minEdgeCount, int maxEdgeCount, bool expected)
{
    single_contradict_test(&create_min_edge_count_fact, minEdgeCount, 
    &create_max_edge_count_fact, maxEdgeCount,
    &delete_min_vertex_count_fact, &delete_max_vertex_count_fact, expected);
}

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

void test_fact(void)
{
    test_contradict();
    test_results();
}