#include "implication.h"
#define KNOWN_IMPLICATIONS_NUMBER 2

typedef bool (*implication_occurs_fun) (int *, int *); 

typedef struct Implication_side {
    bool types[FACT_TYPE_NUM];
    int n_facts;
    int n_params;
    int type_to_param_idx[FACT_TYPE_NUM][MAX_PARAMS_IN_FACT];
} Implication_side;
typedef struct Implication {
    implication_occurs_fun occurs;
    Implication_side left_side;
    Implication_side right_side;
} Implication;

bool implication_type_1_occurs(int *left_side_params, int *right_side_params)
{
    GTD_UNUSED(left_side_params);
    GTD_UNUSED(right_side_params);
    return false;
}

bool implication_type_2_occurs(int *left_side_params, int *right_side_params)
{
    GTD_UNUSED(left_side_params);
    GTD_UNUSED(right_side_params);
    return false;
}

Implication knownImplicationsArray[KNOWN_IMPLICATIONS_NUMBER] = {
    {

    },
    {

    }
};

Fact *implies(Fact *fact)
{
    switch (fact->type)
    {
    case MinEdgeCountFact:
        return create_max_vertex_count_fact(ceil((1 + sqrt(1 + 8 * fact->params[0])) / 2));
    case MaxVertexCountFact:
        return create_max_edge_count_fact((fact->params[0]) * (fact->params[0] - 1) / 2);
    default:
        return NULL;
    }
}