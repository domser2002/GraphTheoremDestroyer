#include "implication.h"
#define KNOWN_IMPLICATIONS_NUMBER 2
#define MAX_LEFT_SIDE_FACTS 1
#define MAX_RIGHT_SIDE_FACTS 1
#define EMPTY_TYPE_TO_PARAM {{-1}, {-1}, {-1}, {-1}}

typedef void (*calc_right_side_params_fun)(int *, int *);

typedef struct Implication_Left_Side
{
    bool types[FACT_TYPE_NUM];
    int n_facts;
    int n_params;
    int type_to_param_idx[FACT_TYPE_NUM][MAX_PARAMS_IN_FACT];
} Implication_Left_Side;

typedef struct Implication_Right_Side
{
    int n_facts;
    int n_params;
    FactType types[MAX_RIGHT_SIDE_FACTS];
} Implication_Right_Side;
typedef struct Implication
{
    calc_right_side_params_fun calculate_params;
    Implication_Left_Side left_side;
    Implication_Right_Side right_side;
} Implication;

/**
 * \brief implication type 1 - MinEdgeCount implies MinVertexCount
 * \param left_side_params - 1 element array containing MinEdgeCount
 * \param right_side_params - 1 element array for MinVertexCount
*/
void implication_type_1_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    int m = left_side_params[0];
    right_side_params[0] = m == 0 ? 0 : ceil((1+sqrt(1+8*m))/2);
    GTD_LOG("Calculated that minEdgeCount = %d implies minVertexCount = %d",left_side_params[0],right_side_params[0]);
}

/**
 * \brief implication type 2 - MaxVertexCount implies MaxEdgeCount
 * \param left_side_params - 1 element array containing MaxVertexCount
 * \param right_side_params - 1 element array for MaxEdgeCount
*/
void implication_type_2_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    int n = left_side_params[0];
    right_side_params[0] = n*(n-1) / 2;
    GTD_LOG("Calculated that maxVertexCount = %d implies maxEdgeCount = %d",left_side_params[0],right_side_params[0]);
}

Implication knownImplicationsArray[KNOWN_IMPLICATIONS_NUMBER] = {
    {
        .left_side = {
        .types = {0},
        .types[MinEdgeCountFact] = true,
        .n_facts = 1,
        .n_params = 1,
        .type_to_param_idx = EMPTY_TYPE_TO_PARAM,
        .type_to_param_idx[MinEdgeCountFact] = {0}
        },
        .right_side = {
            .n_facts = 1,
            .n_params = 1,
            .types = {MinVertexCountFact}
        },
        .calculate_params = &implication_type_1_calculate_right_side_params
    },
    {
        .left_side = {
            .types = {0},
            .types[MaxVertexCountFact] = true,
            .n_facts = 1, 
            .n_params = 1, 
            .type_to_param_idx = EMPTY_TYPE_TO_PARAM,
            .type_to_param_idx[MaxVertexCountFact] = {0}
        }, 
        .right_side = {
            .types = {MaxEdgeCountFact}, 
            .n_params = 1,
            .n_facts = 1
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
    GTD_LOG("Checking if array of %d facts forms a known implication", n_facts);
    *count = 0;
    if (n_facts > MAX_LEFT_SIDE_FACTS)
    {
        GTD_LOG("No implication - too many left side facts");
        return NULL;
    }
    for (int i = 0; i < KNOWN_IMPLICATIONS_NUMBER; i++)
    {
        GTD_LOG("Checking implication number %d", i+1);
        if (knownImplicationsArray[i].left_side.n_facts != n_facts)
        {
            GTD_LOG("Number of left side facts does not match");
            continue;
        }
        int *params = (int *)gtd_malloc(knownImplicationsArray[i].left_side.n_params * sizeof(int));
        bool fact_types_match = true;
        for (int j = 0; j < n_facts; j++)
        {
            if (!knownImplicationsArray[i].left_side.types[factArray[j]->type])
            {
                GTD_LOG("Types of facts do not match");
                fact_types_match = false;
                break;
            }
            // fill params array at the correct positions
            int param_idxs[MAX_PARAMS_IN_FACT];
            for (int k = 0; k < MAX_PARAMS_IN_FACT; k++)
            {
                param_idxs[k] = knownImplicationsArray[i].left_side.type_to_param_idx[factArray[j]->type][k];
            }
            for (int k = 0; k < factArray[j]->params_count; k++)
            {
                params[param_idxs[k]] = factArray[j]->params[k];
            }
        }
        if(fact_types_match)
        {
            GTD_LOG("Implication occurs");
            int *right_side_params = (int*)gtd_malloc(knownImplicationsArray[i].right_side.n_params * sizeof(int));
            knownImplicationsArray[i].calculate_params(params,right_side_params);
            *count = knownImplicationsArray[i].right_side.n_facts;
            Fact **right_side_facts = (Fact**)gtd_malloc(*count * sizeof(Fact*));
            int counter = 0;
            for(int k=0;k<knownImplicationsArray[i].right_side.n_facts;k++)
            {
                FactType type = knownImplicationsArray[i].right_side.types[k];
                int n_params = get_param_count(type);
                int *fact_params = (int*)gtd_malloc(n_params * sizeof(int));
                for(int s=0;s<n_params;s++)
                {
                    fact_params[s] = right_side_params[counter++];
                }
                right_side_facts[k] = create_fact(type,fact_params,n_params);
                gtd_free(fact_params);
                gtd_free(params);
                return right_side_facts;
            }
        }
        gtd_free(params);
    }
    return NULL;
}