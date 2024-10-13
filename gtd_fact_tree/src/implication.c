#include "implication.h"
#define KNOWN_IMPLICATIONS_NUMBER 2

typedef bool (*calc_right_side_params_fun) (int *, int *); 

typedef struct Implication_Side {
    bool types[FACT_TYPE_NUM];
    int n_facts;
    int n_params;
    int type_to_param_idx[FACT_TYPE_NUM][MAX_PARAMS_IN_FACT];
} Implication_Side;
typedef struct Implication {
    calc_right_side_params_fun calculate_params;
    Implication_Side left_side;
    Implication_Side right_side;
} Implication;

bool implication_type_1_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    GTD_UNUSED(left_side_params);
    GTD_UNUSED(right_side_params);
    return false;
}

bool implication_type_2_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    GTD_UNUSED(left_side_params);
    GTD_UNUSED(right_side_params);
    return false;
}

Implication knownImplicationsArray[KNOWN_IMPLICATIONS_NUMBER] = {
    {
        .left_side = {
            .types = {false, false, true, false},
            .n_facts = 1,
            .n_params = 1,
            .type_to_param_idx = {{-1},{-1},{0},{-1}}
        },
        .right_side = {
            .types = {false, true, false, false},
            .n_facts = 1,
            .n_params = 1,
            .type_to_param_idx = {{-1},{0},{-1},{-1}}            
        },
        .calculate_params = &implication_type_1_calculate_right_side_params
    },
    {
        .left_side = {
            .types = {false, true, false, false},
            .n_facts = 1,
            .n_params = 1,
            .type_to_param_idx = {{-1},{0},{-1},{-1}}
        },
        .right_side = {
            .types = {false, false, false, true},
            .n_facts = 1,
            .n_params = 1,
            .type_to_param_idx = {{-1},{-1},{-1},{0}}            
        },
        .calculate_params = &implication_type_2_calculate_right_side_params
    }
};

/**
 * \brief function that checks if array of facts is the left side of a known implication
 *  and returns right side of it in that case
 * \param factArray array of facts (potential left side)
 * \param n_facts number of elements in factArray
 * \param count pointer to return number of elements on the right side
 * \return right side of implication or NULL if there is no implication with that left side defined
*/
Fact **implies(Fact **factArray, int n_facts, int *count)
{
    GTD_UNUSED(factArray);
    count = 0;
    GTD_UNUSED(count);
    return NULL;
    // switch (fact->type)
    // {
    // case MinEdgeCountFact:
    //     return create_max_vertex_count_fact(ceil((1 + sqrt(1 + 8 * fact->params[0])) / 2));
    // case MaxVertexCountFact:
    //     return create_max_edge_count_fact((fact->params[0]) * (fact->params[0] - 1) / 2);
    // default:
    //     return NULL;
    // }
}