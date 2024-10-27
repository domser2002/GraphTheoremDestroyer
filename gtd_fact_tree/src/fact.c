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

static Fact *create_min_vertex_count_fact(int minVertexCount)
{
    return create_one_parameter_fact(MinVertexCountFact, minVertexCount);
}

static Fact *create_max_vertex_count_fact(int maxVertexCount)
{
    return create_one_parameter_fact(MaxVertexCountFact, maxVertexCount);
}

static Fact *create_min_edge_count_fact(int minEdgeCount)
{
    return create_one_parameter_fact(MinEdgeCountFact, minEdgeCount);
}

static Fact *create_max_edge_count_fact(int maxEdgeCount)
{
    return create_one_parameter_fact(MaxEdgeCountFact, maxEdgeCount);
}

/**
 * \brief constructor for Fact class
*/
Fact *create_fact(FactType type, int *params, int params_count)
{
    switch (type)
    {
    case MinVertexCountFact:
        if(params_count != 1) return NULL;
        return create_min_vertex_count_fact(params[0]);
    case MaxVertexCountFact:
        if(params_count != 1) return NULL;
        return create_max_vertex_count_fact(params[0]);
    case MinEdgeCountFact:
        if(params_count != 1) return NULL;
        return create_min_edge_count_fact(params[0]);
    case MaxEdgeCountFact:
        if(params_count != 1) return NULL;
        return create_max_edge_count_fact(params[0]);
    default:
        return NULL;
    }
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

static int delete_min_vertex_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact, MinVertexCountFact);
}

static int delete_max_vertex_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact, MaxVertexCountFact);
}

static int delete_min_edge_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact, MinEdgeCountFact);
}

static int delete_max_edge_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact, MaxEdgeCountFact);
}

/**
 * \brief destructor for Fact class
*/
int delete_fact(Fact *fact)
{
    switch (fact->type)
    {
    case MinVertexCountFact:
        return delete_min_vertex_count_fact(fact);
    case MaxVertexCountFact:
        return delete_max_vertex_count_fact(fact);
    case MinEdgeCountFact:
        return delete_min_edge_count_fact(fact);
    case MaxEdgeCountFact:
        return delete_max_edge_count_fact(fact);
    default:
        return -1;
    }
}

/**
 * \brief function to check how many parameters are required for fact type
 * \param type fact type
 * \return number of parameters needed
*/
int get_param_count(FactType type)
{
    GTD_UNUSED(type);
    return 1;
}

/**
 * \brief function to check if 2 facts are equal
 * \note needed because Fact fields are protected
 * \param fact1 first fact
 * \param fact2 second fact
 * \return true if facts are equal, false otherwise
*/
bool equal(Fact *fact1, Fact *fact2)
{
    if(fact1->type != fact2->type) return false;
    for(uint8_t i=0;i<fact1->params_count;i++)
    {
        if(fact1->params[i] != fact2->params[i])
            return false;
    }
    return true;
}

/**
 * \brief function to get a string from Fact object
 * \note used for writting the proof
 * \param fact Fact object
 * \return string description of Fact
*/
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
