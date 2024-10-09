#include "test_fact.h"

static void test_contradict(void)
{
    // Arrange
    Fact *fact1 = create_max_vertex_count_fact(1);
    Fact *fact2 = create_min_edge_count_fact(1);
    // Act 
    bool result = contradict(fact1,fact2);
    // Assert
    assert(result == true);
}

static void test_results(void)
{

}

void test_fact(void)
{
    test_contradict();
    test_results();
}