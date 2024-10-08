#include "fact.h"

struct Fact 
{
    FactType type;
    uint32_t *params;
    uint8_t params_count;
};

static Fact *create_one_parameter_fact(FactType type, uint32_t param)
{
    Fact *newFact = gtd_malloc(sizeof(Fact));
    newFact->type = type;
    newFact->params = gtd_malloc(1*sizeof(uint32_t));
    newFact->params[0] = param;
    return newFact;
}

Fact *create_min_vertex_count_fact(int minVertexCount)
{
    return create_one_parameter_fact(MinVertexCountFact,minVertexCount);
}

Fact *create_max_vertex_count_fact(int maxVertexCount)
{
    return create_one_parameter_fact(MaxVertexCountFact,maxVertexCount);
}

Fact *create_min_edge_count_fact(int minEdgeCount)
{
    return create_one_parameter_fact(MinEdgeCountFact,minEdgeCount);
}

Fact *create_max_edge_count_fact(int maxEdgeCount)
{
    return create_one_parameter_fact(MaxEdgeCountFact,maxEdgeCount);
}

static int delete_one_parameter_fact(Fact *fact, FactType type)
{
    if(fact->type != type)
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
    return delete_one_parameter_fact(fact,MinVertexCountFact);
}

int delete_max_vertex_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact,MaxVertexCountFact);
}

int delete_min_edge_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact,MinEdgeCountFact);
}

int delete_max_edge_count_fact(Fact *fact)
{
    return delete_one_parameter_fact(fact,MaxEdgeCountFact);
}

bool contradict(Fact *fact1, Fact *fact2)
{
    switch(fact1->type)
    {
        case MinVertexCountFact:
            switch(fact2->type)
            {
                case MaxVertexCountFact:
                    return fact2->params[0] > fact1->params[0];
                default:
                    return false;
            }
        case MinEdgeCountFact:
            switch(fact2->type)
            {
                case MaxVertexCountFact:
                    return (fact2->params[0]) * (fact2->params[0] - 1) / 2 < fact1->params[0];
                case MaxEdgeCountFact:
                    return fact2->params[0] > fact1->params[0];
                default:
                    return false;                
            }
        default:
            return false;
    }
}

Fact *results(Fact *fact)
{
    switch(fact->type)
    {
        case MinEdgeCountFact:
            return create_max_vertex_count_fact(ceil( (1 + sqrt(1+8*fact->params[0])) / 2) );
        case MaxVertexCountFact:
            return create_max_edge_count_fact((fact->params[0]) * (fact->params[0] - 1) / 2);
        default:
            return NULL;
    }
}

char *get_fact_str(Fact *fact)
{
    char *result = (char*)gtd_malloc(128*sizeof(char));
    switch (fact->type)
    {
        case MinEdgeCountFact:
            sprintf(result,"Graph has at least %d edges", fact->params[0]);
            return result;
        case MaxEdgeCountFact:
            sprintf(result,"Graph has at most %d edges", fact->params[0]);
            return result;
        case MinVertexCountFact:
            sprintf(result,"Graph has at least %d vertices", fact->params[0]);
            return result;
        case MaxVertexCountFact:
            sprintf(result,"Graph has at most %d vertices", fact->params[0]);
            return result;
        default:
            return "";
    }
}
