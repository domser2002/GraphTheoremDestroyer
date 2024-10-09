#include "test_fact.h"

typedef Fact *(*create_fact_func)(int);
typedef int (*delete_fact_func)(Fact *);

static void single_contradict_test(create_fact_func create_fact1, int fact1_param, create_fact_func create_fact2, int fact2_param, delete_fact_func delete_fact1, delete_fact_func delete_fact2, bool expected)
{
    // Arrange
    Fact *fact1 = create_fact1(fact1_param);
    Fact *fact2 = create_fact2(fact2_param);
    // Act
    bool result = contradict(fact1, fact2);
    // Assert
    assert(result == expected);
    // Clean
    delete_fact1(fact1);
    delete_fact2(fact2);
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
    // check if order changes anything
    // TODO: rethink if still needed after implementing new approach
    single_contradict_test(&create_min_edge_count_fact, minEdgeCount, 
    &create_max_vertex_count_fact, maxVertexCount,
    &delete_max_vertex_count_fact, &delete_min_edge_count_fact, expected);
}

static void test_contradict(void)
{
    test_contradiction_1(1,1,true);
    test_contradiction_1(1,0,false);
    test_contradiction_1(0,0,false);
    test_contradiction_1(10000,10000 * 9999 / 2 + 1,true);
    test_contradiction_1(10000,10000 * 9999 / 2,false);

}

static void test_results(void)
{
}

void test_fact(void)
{
    test_contradict();
    test_results();
}