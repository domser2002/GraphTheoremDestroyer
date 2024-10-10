#include "fact.h"
#define MAX_CONTRADICTING_FACTS 2
#define KNOWN_CONTRADICTIONS_NUMBER 2
#define FACT_TYPE_NUM 4
#define MAX_PARAMS_IN_FACT 1

typedef bool (*contradiction_occurs_fun) (int*);

typedef struct Contradiction {
    bool types[FACT_TYPE_NUM];
    contradiction_occurs_fun occurs;
    int n_facts;
    int n_params;
    int type_to_param_idx[FACT_TYPE_NUM][MAX_PARAMS_IN_FACT];
} Contradiction;

struct Fact 
{
    FactType type;
    uint32_t *params;
    uint8_t params_count;
};

/**
 * \brief contradiction type 1 - contradiction between max vertex count and min edge count
 * \param params - array of 2 integers, max vertex count and min edge count
 * \return true if contradiction occurs, false otherwise
*/
static bool contradiction_type_1_occurs(int *params)
{
    return (params[0] * (params[0] - 1) / 2) > params[1];
}

static bool contradiction_type_2_occurs(int *params)
{
    GTD_UNUSED(params);
    return false;
}

const Contradiction knownContradictionsArray[KNOWN_CONTRADICTIONS_NUMBER] = {
    {
        .types = {false, true, true, false},
        .occurs = &contradiction_type_1_occurs,
        .n_facts = 2,
        .n_params = 2,
        .type_to_param_idx = {{-1}, {0}, {1}, {-1}}
    },
    {
        .types = {false, true, true, false},
        .occurs = &contradiction_type_2_occurs,
        .n_facts = 2,
        .n_params = 2,
        .type_to_param_idx = {{-1}, {0} ,{1} ,{-1}}
    }
};

static Fact *create_one_parameter_fact(FactType type, uint32_t param)
{
    Fact *newFact = (Fact *)gtd_malloc(sizeof(Fact));
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

bool contradict(Fact **factArray, int n_facts)
{
    for(int i=0; i < KNOWN_CONTRADICTIONS_NUMBER; i++)
    {
        int count = 0;
        int *params = (int*)gtd_malloc(knownContradictionsArray[i].n_params * sizeof(int));
        for(int j=0;j<n_facts;j++)
        {
            if(knownContradictionsArray[i].types[factArray[j]->type])
            {
                count++;
                int param_idxs[MAX_PARAMS_IN_FACT];
                for (int k = 0; k < MAX_PARAMS_IN_FACT; k++) {
                    param_idxs[k] = knownContradictionsArray[i].type_to_param_idx[factArray[j]->type][k];
                }
                for(int k=0; k < factArray[j]->params_count; k++)
                {
                    params[param_idxs[k]] = factArray[j]->params[k];
                }
            }
        }
        if(count == knownContradictionsArray[i].n_facts)
        {
            if(knownContradictionsArray[i].occurs(params))
                return true;
        }
        gtd_free(params);
    }
    return false;
    // switch(fact1->type)
    // {
    //     case MinVertexCountFact:
    //         switch(fact2->type)
    //         {
    //             case MaxVertexCountFact:
    //                 return fact2->params[0] > fact1->params[0];
    //             default:
    //                 return false;
    //         }
    //     case MinEdgeCountFact:
    //         switch(fact2->type)
    //         {
    //             case MaxVertexCountFact:
    //                 return (fact2->params[0]) * (fact2->params[0] - 1) / 2 < fact1->params[0];
    //             case MaxEdgeCountFact:
    //                 return fact2->params[0] > fact1->params[0];
    //             default:
    //                 return false;                
    //         }
    //     default:
    //         return false;
    // }
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
