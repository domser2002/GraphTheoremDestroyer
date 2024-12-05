#include "contradiction.h"
#define KNOWN_CONTRADICTIONS_NUMBER 9

typedef bool (*contradiction_occurs_fun)(int *);

typedef struct Contradiction
{
    bool types[FACT_TYPE_NUM];
    contradiction_occurs_fun occurs;
    int n_facts;
    int n_params;
    int type_to_param_idx[FACT_TYPE_NUM][MAX_PARAMS_IN_FACT];
} Contradiction;

const Contradiction EMPTY_CONTRADICTION = 
{
    .types = {0},
    .occurs = NULL,
    .n_facts = -1,
    .n_params = -1,
    .type_to_param_idx = {
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}
    }
};


/**
 * \brief contradiction type 1 - contradiction between max vertex count and min edge count
 * \param params - array of 2 integers, max vertex count and min edge count
 * \return true if contradiction occurs, false otherwise
 */
static bool contradiction_type_1_occurs(int *params)
{
    GTD_LOG("Checking for contradiction type 1, maxVertexCount = %d, minEdgeCount = %d", params[0],params[1]);
    return (params[0] * (params[0] - 1) / 2) < params[1];
}

/**
 * \brief contradiction type 2 - contradiction between min vertex count and max vertex count
 * \param params - array of 2 integers, min vertex count and max vertex count
 * \return true if contradiction occurs, false otherwise
 */
static bool contradiction_type_2_occurs(int *params)
{
    GTD_LOG("Checking for contradiction type 2, minVertexCount = %d, maxVertexCount = %d", params[0],params[1]);
    return params[0] > params[1];
}

/**
 * \brief contradiction type 3 - contradiction between min edge count and max edge count
 * \param params - array of 2 integers, min edge count and max edge count
 * \return true if contradiction occurs, false otherwise
 */
static bool contradiction_type_3_occurs(int *params)
{
    GTD_LOG("Checking for contradiction type 2, minEdgeCount = %d, maxEdgeCount = %d", params[0],params[1]);
    return params[0] > params[1];
}

/**
 * \brief contradiction type 5 - contradiction between planar graphs and K_{3,3} as a minor
 * \param params - array of 3 integers, number of partition classes (only 2 is supoorted) and their count 
 * \return true if contradiction occurs (minor actually is K_{3,3} or bigger), false otherwise
 */
static bool contradiction_type_5_occurs(int *params)
{
    GTD_LOG("Checking for contradiction type 5, partitionClassesNum = %d, count1 = %d, count2 = %d", params[0], params[1], params[2]);
    return params[0] == 2 && params[1] >= 3 && params[2] >= 3;
}

/**
 * \brief contradiction type 6 - contradiction between planar graphs and K_{5} as a minor
 * \param params - array of 1 integer, clique size
 * \return true if contradiction occurs (minor actually is K_{5} or bigger), false otherwise
*/
static bool contradiction_type_6_occurs(int *params)
{
    GTD_LOG("Checking for contradiction type 6, clique size = %d", params[0]);
    return params[0] >= 5;
}

/**
 * \brief contradiction type 7 - contradiction between graph having a cycle and having no cycles
 * \param params - array of 1 integer, cycle size, unused
 * \return always true
*/
static bool contradiction_type_7_occurs(int *params)
{
    GTD_UNUSED(params);
    GTD_LOG("Not checking for contradiction type 7, it always occurs");
    return true;
}

/**
 * \brief contradiction type 9 - bipartite graphs do not contain odd cycles
 * \param params - array of 2 integers, k1 meaning k1-partite graph, k2 meaning cycle length
 * \return true if bipartite and odd cycle, false otherwise
*/
static bool contradiction_type_9_occurs(int *params)
{
    GTD_LOG("Checking for contradiction type 9, %d-partite graph has a cycle with length %d", params[0],params[1]);
    return params[0] == 2 && params[1] % 2 == 1;
}

// according to README
const Contradiction knownContradictionsArray[KNOWN_CONTRADICTIONS_NUMBER] = {
    [0] = EMPTY_CONTRADICTION,
    [1] = EMPTY_CONTRADICTION,
    [2] = EMPTY_CONTRADICTION,
    [3] = EMPTY_CONTRADICTION,
    [4] = EMPTY_CONTRADICTION,
    [5] = EMPTY_CONTRADICTION,
    [6] = EMPTY_CONTRADICTION,
    [7] = EMPTY_CONTRADICTION,
    [8] = EMPTY_CONTRADICTION,
    // type 1
    [0].types[MaxVertexCountFact] = true,
    [0].types[MinEdgeCountFact] = true,
    [0].occurs = &contradiction_type_1_occurs,
    [0].n_facts = 2,
    [0].n_params = 2,
    [0].type_to_param_idx[MaxVertexCountFact] = {0},
    [0].type_to_param_idx[MinEdgeCountFact] = {1},
    // type 2
    [1].types[MinVertexCountFact] = true,
    [1].types[MaxVertexCountFact] = true,
    [1].occurs = &contradiction_type_2_occurs,
    [1].n_facts = 2,
    [1].n_params = 2,
    [1].type_to_param_idx[MinVertexCountFact] = {0},
    [1].type_to_param_idx[MaxVertexCountFact] = {1},
    // type 3
    [2].types[MinEdgeCountFact] = true,
    [2].types[MaxEdgeCountFact] = true,
    [2].occurs = &contradiction_type_3_occurs,
    [2].n_facts = 2,
    [2].n_params = 2,
    [2].type_to_param_idx[MinEdgeCountFact] = {0},
    [2].type_to_param_idx[MaxEdgeCountFact] = {1},
    // type 4
    // valid only with functions, will be added later
    // type 5
    [4].types[IsPlanarFact] = true,
    [4].types[HasMinorCompletePartiteFact] = true,
    [4].occurs = &contradiction_type_5_occurs,
    [4].n_facts = 2,
    [4].n_params = 3,
    [4].type_to_param_idx[IsPlanarFact] = {},
    [4].type_to_param_idx[HasMinorCompletePartiteFact] = {0,1,2},
    // type 6
    [5].types[IsPlanarFact] = true,
    [5].types[HasMinorCliqueFact] = true,
    [5].occurs = &contradiction_type_6_occurs,
    [5].n_facts = 2,
    [5].n_params = 1,
    [5].type_to_param_idx[IsPlanarFact] = {},
    [5].type_to_param_idx[HasMinorCliqueFact] = {0},
    // type 7
    [6].types[HasNoCyclesFact] = true,
    [6].types[HasCycleFact] = true,
    [6].occurs = &contradiction_type_7_occurs,
    [6].n_facts = 2,
    [6].n_params = 1,
    [6].type_to_param_idx[HasNoCyclesFact] = {},
    [6].type_to_param_idx[HasCycleFact] = {0},
    // type 8 
    // valid only with functions, will be added later
    // type 9
    [8].types[IsPartiteFact] = true,
    [8].types[HasCycleFact] = true,
    [8].occurs = &contradiction_type_9_occurs,
    [8].n_facts = 2,
    [8].n_params = 2,
    [8].type_to_param_idx[IsPartiteFact] = {0},
    [8].type_to_param_idx[HasCycleFact] = {1}
    };

/**
 * \brief Function which checks if facts from factArray are in one of the predefined contradictions
 * \param factArray array of n_facts Fact objects, fact types should not duplicate, if they do the behaviour is undefined
 * \param n_facts number of facts in array, should be <= MAX_CONTRADICTING_FACTS
 * \return true if facts are in one of the predefined contradictions, false otherwise
 */
bool contradict(Fact **factArray, int n_facts)
{
    GTD_LOG("Checking if array of %d facts forms a known contradiction",n_facts);
    if (n_facts > MAX_CONTRADICTING_FACTS)
    {
        GTD_LOG("No contradiction - too many facts");
        return false;
    }
    for (int i = 0; i < KNOWN_CONTRADICTIONS_NUMBER; i++)
    {
        GTD_LOG("Checking contradiction number %d", i+1);
        if (n_facts != knownContradictionsArray[i].n_facts)
        {
            GTD_LOG("Number of facts does not match");
            continue;
        }
        int *params = (int *)gtd_malloc(knownContradictionsArray[i].n_params * sizeof(int));
        bool fact_types_match = true;
        for (int j = 0; j < n_facts; j++)
        {
            if (!knownContradictionsArray[i].types[factArray[j]->type])
            {
                GTD_LOG("Types of facts do not match");
                fact_types_match = false;
                break;
            }
            else
            {
                // fill params array at the correct positions
                int param_idxs[MAX_PARAMS_IN_FACT];
                for (int k = 0; k < MAX_PARAMS_IN_FACT; k++)
                {
                    param_idxs[k] = knownContradictionsArray[i].type_to_param_idx[factArray[j]->type][k];
                }
                for (int k = 0; k < factArray[j]->params_count; k++)
                {
                    params[param_idxs[k]] = factArray[j]->params[k];
                }
            }
        }
        if (fact_types_match && knownContradictionsArray[i].occurs(params))
        {
            GTD_LOG("Contradiction occurs");
            return true;
        }
        gtd_free(params);
    }
    return false;
}