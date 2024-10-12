#include "fact.h"

struct Fact
{
    FactType type;
    uint32_t *params;
    uint8_t params_count;
};

static Fact *create_one_parameter_fact(FactType type, uint32_t param)
{
    Fact *newFact = (Fact *)gtd_malloc(sizeof(Fact));
    newFact->type = type;
    newFact->params = gtd_malloc(1 * sizeof(uint32_t));
    newFact->params[0] = param;
    newFact->params_count = 1;
    return newFact;
}

Fact *create_min_vertex_count_fact(int minVertexCount)
{
    return create_one_parameter_fact(MinVertexCountFact, minVertexCount);
}

Fact *create_max_vertex_count_fact(int maxVertexCount)
{
    return create_one_parameter_fact(MaxVertexCountFact, maxVertexCount);
}

Fact *create_min_edge_count_fact(int minEdgeCount)
{
    return create_one_parameter_fact(MinEdgeCountFact, minEdgeCount);
}

Fact *create_max_edge_count_fact(int maxEdgeCount)
{
    return create_one_parameter_fact(MaxEdgeCountFact, maxEdgeCount);
}

static int delete_one_parameter_fact(Fact *fact, FactType type)
{
    if (fact->type != type)
    {
        GTD_LOG("[ERROR] Wrong fact type!");
        return 1;
    }
    gtd_free(fact->params);
    gtd_free(fact);
    return 0;
}

int delete_min_vertex_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact, MinVertexCountFact);
}

int delete_max_vertex_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact, MaxVertexCountFact);
}

int delete_min_edge_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact, MinEdgeCountFact);
}

int delete_max_edge_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact, MaxEdgeCountFact);
}

char *get_fact_str(Fact *fact)
{
    char *result = (char *)gtd_malloc(128 * sizeof(char));
    switch (fact->type)
    {
    case MinEdgeCountFact:
        sprintf(result, "Graph has at least %d edges", fact->params[0]);
        return result;
    case MaxEdgeCountFact:
        sprintf(result, "Graph has at most %d edges", fact->params[0]);
        return result;
    case MinVertexCountFact:
        sprintf(result, "Graph has at least %d vertices", fact->params[0]);
        return result;
    case MaxVertexCountFact:
        sprintf(result, "Graph has at most %d vertices", fact->params[0]);
        return result;
    default:
        return "";
    }
}
