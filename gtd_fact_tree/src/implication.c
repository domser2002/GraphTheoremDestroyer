#include "implication.h"
#define KNOWN_IMPLICATIONS_NUMBER 17
#define MAX_RIGHT_SIDE_FACTS 2

typedef bool (*calc_right_side_params_fun)(int *, int *);

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

const Implication EMPTY_IMPLICATION = 
{
    .left_side = {
        .types = {0},
        .n_facts = -1,
        .n_params = -1,
        .type_to_param_idx = {
        {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
        {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
        {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
        {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
        {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}
    }
    },
    .right_side = {
        .n_facts = -1,
        .n_params = -1,
        .types = {0}
    },
    .calculate_params = NULL
};

/**
 * \brief implication type 1 - EdgeCount implies MinEdgeCount and MaxEdgeCount
 * \param left_side_params - 1 element array containing EdgeCount
 * \param right_side_params - 2 element array for MinEdgeCount and MaxEdgeCount
*/
bool implication_type_1_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    right_side_params[0] = left_side_params[0];
    right_side_params[1] = left_side_params[0];
    GTD_LOG("Calculated that EdgeCount = %d implies minEdgeCount = %d and maxEdgeCount = %d",left_side_params[0],right_side_params[0], right_side_params[1]);
    return true;
}

/**
 * \brief implication type 2 - VertexCount implies MinVertexCount and MaxVertexCount
 * \param left_side_params - 1 element array containing VertexCount
 * \param right_side_params - 2 element array for MinVertexCount and MaxVertexCount
*/
bool implication_type_2_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    right_side_params[0] = left_side_params[0];
    right_side_params[1] = left_side_params[0];
    GTD_LOG("Calculated that VertexCount = %d implies minVertexCount = %d and maxVertexCount = %d",left_side_params[0],right_side_params[0], right_side_params[1]);
    return true;
}

/**
 * \brief implication type 3 - MinEdgeCount implies MinVertexCount
 * \param left_side_params - 1 element array containing MinEdgeCount
 * \param right_side_params - 1 element array for MinVertexCount
*/
bool implication_type_3_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    int m = left_side_params[0];
    right_side_params[0] = m == 0 ? 0 : ceil((1+sqrt(1+8*m))/2);
    GTD_LOG("Calculated that minEdgeCount = %d implies minVertexCount = %d",left_side_params[0],right_side_params[0]);
    return true;
}

/**
 * \brief implication type 4 - MaxVertexCount implies MaxEdgeCount
 * \param left_side_params - 1 element array containing MaxVertexCount
 * \param right_side_params - 1 element array for MaxEdgeCount
*/
bool implication_type_4_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    int n = left_side_params[0];
    right_side_params[0] = n*(n-1) / 2;
    GTD_LOG("Calculated that maxVertexCount = %d implies maxEdgeCount = %d",left_side_params[0],right_side_params[0]);
    return true;
}

/**
 * \brief implication type 5 - is tree implies is connected and has no cycles
 * \param left_side_params - 0 element array
 * \param right_side_params - 0 element array
*/
bool implication_type_5_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    GTD_UNUSED(left_side_params);
    GTD_UNUSED(right_side_params);
    GTD_LOG("Is tree implies is connected and has no cycles");
    return true;
}

/**
 * \brief implication type 6 - is connected and has no cycles implies is tree
 * \param left_side_params - 0 element array
 * \param right_side_params - 0 element array
*/
bool implication_type_6_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    GTD_UNUSED(left_side_params);
    GTD_UNUSED(right_side_params);
    GTD_LOG("Is connected and has no cycles implies is tree");
    return true;
}

/**
 * \brief implication type 8 - is tree, has no induced complete partite implies is t-nary tree
 * \param left_side_params - 3 element array, describing complete partite graph 
 * \param right_side_params - 1 element array for t
*/
bool implication_type_8_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    if(left_side_params[0] != 2 || left_side_params[1] != 1)
        return false;
    right_side_params[0] = left_side_params[2] - 1;
    GTD_LOG("Calculated that is tree and contains induced K_{%d,%d} implies is %d-nary tree",left_side_params[1],left_side_params[2],right_side_params[0]);
    return true;
}

/**
 * \brief implication type 9 - is tree, has no induced path implies max tree height
 * \param left_side_params - 1 element array, path length 
 * \param right_side_params - 1 element array for max tree height
*/
bool implication_type_9_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    right_side_params[0] = left_side_params[0] - 1;
    GTD_LOG("Calculated that is tree and contains induced P_{%d} implies is max tree is %d",left_side_params[0],right_side_params[0]);
    return true;
}

/**
 * \brief implication type 10 - is t-nary tree with max tree height implies max vertex count
 * \param left_side_params - 2 element array, t and h
 * \param right_side_params - 1 element array for max vertex count
*/
bool implication_type_10_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    int t = left_side_params[0];
    int h = left_side_params[1];
    // (t^h-1)/(t-1)
    int n = 1;
    for(int i=0;i<h;i++)
        n *= t;
    n = (n-1)/(t-1);
    right_side_params[0] = n;
    GTD_LOG("Calculated that is %d-nary tree with max height = %d implies is max vertex count = %d",t,h,n);
    return true;
}

/**
 * \brief implication type 11 - no K_{3,3} and K_{5} as minors implies is planar
 * \param left_side_params - 4 element array, describing complete partite graph and clique size
 * \param right_side_params - 0 element array
*/
bool implication_type_11_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    GTD_UNUSED(right_side_params);
    if(left_side_params[0] != 2 || left_side_params[1] > 3 || left_side_params[2] > 3 || left_side_params[3] > 5)
        return false;
    GTD_LOG("Checked that does not contain K_{%d,%d} and K_{%d} implies is planar",left_side_params[1],left_side_params[2],left_side_params[3]);
    return true;
}

/**
 * \brief implication type 14 - cycle complement graphs with n <= 6 are planar
 * \param left_side_params - 1 element array, max vertex count
 * \param right_side_params - 0 element array
*/
bool implication_type_14_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    GTD_UNUSED(right_side_params);
    if(left_side_params[0] > 6)
        return false;
    GTD_LOG("Checked that cycle complement graph with %d vertices is planar",left_side_params[0]);
    return true;
}

/**
 * \brief implication type 15 - cycle complement graph with n = 7 contains K_{3,3} as minor
 * \param left_side_params - 1 element array, vertex count
 * \param right_side_params - 3 element array, describing complete partite graph
*/
bool implication_type_15_calculate_right_side_params(int *left_side_params, int *right_side_params)
{
    if(left_side_params[0] != 7)
        return false;
    right_side_params[0] = 2;
    right_side_params[1] = 3;
    right_side_params[2] = 3;
    GTD_LOG("Cycle complement graph with 7 vertices contains K_{3,3}");
    return true;
}

// according to README
Implication knownImplicationsArray[KNOWN_IMPLICATIONS_NUMBER] = {
    [0] = EMPTY_IMPLICATION,
    [1] = EMPTY_IMPLICATION,
    [2] = EMPTY_IMPLICATION,
    [3] = EMPTY_IMPLICATION,
    [4] = EMPTY_IMPLICATION,
    [5] = EMPTY_IMPLICATION,
    [6] = EMPTY_IMPLICATION,
    [7] = EMPTY_IMPLICATION,
    [8] = EMPTY_IMPLICATION,
    [9] = EMPTY_IMPLICATION,
    [10] = EMPTY_IMPLICATION,
    [11] = EMPTY_IMPLICATION,
    [12] = EMPTY_IMPLICATION,
    [13] = EMPTY_IMPLICATION,
    [14] = EMPTY_IMPLICATION,
    [15] = EMPTY_IMPLICATION,
    [16] = EMPTY_IMPLICATION,

    // type 1
    [0].left_side.types[EdgeCountFact] = true,
    [0].left_side.n_facts = 1,
    [0].left_side.n_params = 1,
    [0].left_side.type_to_param_idx[EdgeCountFact] = {0},
    [0].right_side.n_facts = 2,
    [0].right_side.n_params = 2,
    [0].right_side.types = {MinEdgeCountFact, MaxEdgeCountFact},
    [0].calculate_params = &implication_type_1_calculate_right_side_params,
    // type 2
    [1].left_side.types[VertexCountFact] = true,
    [1].left_side.n_facts = 1,
    [1].left_side.n_params = 1,
    [1].left_side.type_to_param_idx[VertexCountFact] = {0},
    [1].right_side.n_facts = 2,
    [1].right_side.n_params = 2,
    [1].right_side.types = {MinVertexCountFact, MaxVertexCountFact},
    [1].calculate_params = &implication_type_2_calculate_right_side_params,
    // type 3
    [2].left_side.types[MinEdgeCountFact] = true,
    [2].left_side.n_facts = 1,
    [2].left_side.n_params = 1,
    [2].left_side.type_to_param_idx[MinEdgeCountFact] = {0},
    [2].right_side.n_facts = 1,
    [2].right_side.n_params = 1,
    [2].right_side.types = {MinVertexCountFact},
    [2].calculate_params = &implication_type_3_calculate_right_side_params,
    // type 4
    [3].left_side.types[MaxVertexCountFact] = true,
    [3].left_side.n_facts = 1, 
    [3].left_side.n_params = 1, 
    [3].left_side.type_to_param_idx[MaxVertexCountFact] = {0},
    [3].right_side.types = {MaxEdgeCountFact}, 
    [3].right_side.n_params = 1,
    [3].right_side.n_facts = 1,
    [3].calculate_params = &implication_type_4_calculate_right_side_params,
    // type 5
    [4].left_side.types[IsTreeFact] = true,
    [4].left_side.n_facts = 1,
    [4].left_side.n_params = 0,
    [4].right_side.n_facts = 2,
    [4].right_side.n_params = 0,
    [4].right_side.types = {IsConnectedFact, HasNoCyclesFact},
    [4].calculate_params = &implication_type_5_calculate_right_side_params,
    // type 6
    [5].left_side.types[IsConnectedFact] = true,
    [5].left_side.types[HasNoCyclesFact] = true,
    [5].left_side.n_facts = 2,
    [5].left_side.n_params = 0,
    [5].right_side.n_facts = 1,
    [5].right_side.n_params = 0,
    [5].right_side.types = {IsTreeFact},
    [5].calculate_params = &implication_type_6_calculate_right_side_params,
    // type 7
    // only with functions, will be added later
    // type 8
    [7].left_side.types[IsTreeFact] = true,
    [7].left_side.types[HasNoInducedCompletePartiteFact] = true,
    [7].left_side.n_facts = 2,
    [7].left_side.n_params = 3,
    [7].left_side.type_to_param_idx[HasNoInducedCompletePartiteFact] = {0,1,2},
    [7].right_side.n_facts = 1,
    [7].right_side.n_params = 1,
    [7].right_side.types = {IstnaryTreeFact},
    [7].calculate_params = &implication_type_8_calculate_right_side_params,
    // type 9
    [8].left_side.types[IsTreeFact] = true,
    [8].left_side.types[HasNoInducedPathFact] = true,
    [8].left_side.n_facts = 2,
    [8].left_side.n_params = 1,
    [8].left_side.type_to_param_idx[HasNoInducedPathFact] = {0},
    [8].right_side.n_facts = 1,
    [8].right_side.n_params = 1,
    [8].right_side.types = {MaxTreeHeightFact},
    [8].calculate_params = &implication_type_9_calculate_right_side_params,
    // type 10
    [9].left_side.types[IstnaryTreeFact] = true,
    [9].left_side.types[MaxTreeHeightFact] = true,
    [9].left_side.n_facts = 2,
    [9].left_side.n_params = 2,
    [9].left_side.type_to_param_idx[IstnaryTreeFact] = {0},
    [9].left_side.type_to_param_idx[MaxTreeHeightFact] = {1},
    [9].right_side.n_facts = 1,
    [9].right_side.n_params = 1,
    [9].right_side.types = {MaxVertexCountFact},
    [9].calculate_params = &implication_type_10_calculate_right_side_params,
    // type 11
    [10].left_side.types[HasNoMinorCompletePartiteFact] = true,
    [10].left_side.types[HasNoMinorCliqueFact] = true,
    [10].left_side.n_facts = 2,
    [10].left_side.n_params = 4,
    [10].left_side.type_to_param_idx[HasNoMinorCompletePartiteFact] = {0,1,2},
    [10].left_side.type_to_param_idx[HasNoMinorCliqueFact] = {3},
    [10].right_side.n_facts = 1,
    [10].right_side.n_params = 0,
    [10].right_side.types = {IsPlanarFact},
    [10].calculate_params = &implication_type_11_calculate_right_side_params,
    // type 12
    // only with functions, will be added later
    // type 13
    // only with functions, will be added later
    // type 14
    [13].left_side.types[IsCycleComplementFact] = true,
    [13].left_side.types[MaxVertexCountFact] = true,
    [13].left_side.n_facts = 2,
    [13].left_side.n_params = 1,
    [13].left_side.type_to_param_idx[MaxVertexCountFact] = {0},
    [13].right_side.n_facts = 1,
    [13].right_side.n_params = 0,
    [13].right_side.types = {IsPlanarFact},
    [13].calculate_params = &implication_type_14_calculate_right_side_params,
    // type 15
    [14].left_side.types[IsCycleComplementFact] = true,
    [14].left_side.types[VertexCountFact] = true,
    [14].left_side.n_facts = 2,
    [14].left_side.n_params = 1,
    [14].left_side.type_to_param_idx[VertexCountFact] = {0},
    [14].right_side.n_facts = 1,
    [14].right_side.n_params = 3,
    [14].right_side.types = {HasMinorCompletePartiteFact},
    [14].calculate_params = &implication_type_15_calculate_right_side_params
    // type 16
    // only with functions, will be added later
    // type 17
    // only with functions, will be added later
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