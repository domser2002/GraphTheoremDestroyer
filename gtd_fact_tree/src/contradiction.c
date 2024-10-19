#include "contradiction.h"
#define MAX_CONTRADICTING_FACTS 2
#define KNOWN_CONTRADICTIONS_NUMBER 3

typedef bool (*contradiction_occurs_fun)(int *);

typedef struct Contradiction
{
    bool types[FACT_TYPE_NUM];
    contradiction_occurs_fun occurs;
    int n_facts;
    int n_params;
    int type_to_param_idx[FACT_TYPE_NUM][MAX_PARAMS_IN_FACT];
} Contradiction;

/**
 * \brief contradiction type 1 - contradiction between max vertex count and min edge count
 * \param params - array of 2 integers, max vertex count and min edge count
 * \return true if contradiction occurs, false otherwise
 */
static bool contradiction_type_1_occurs(int *params)
{
    return (params[0] * (params[0] - 1) / 2) < params[1];
}

/**
 * \brief contradiction type 2 - contradiction between min vertex count and max vertex count
 * \param params - array of 2 integers, min vertex count and max vertex count
 * \return true if contradiction occurs, false otherwise
 */
static bool contradiction_type_2_occurs(int *params)
{
    return params[0] > params[1];
}

/**
 * \brief contradiction type 3 - contradiction between min edge count and max edge count
 * \param params - array of 2 integers, min edge count and max edge count
 * \return true if contradiction occurs, false otherwise
 */
static bool contradiction_type_3_occurs(int *params)
{
    return params[0] > params[1];
}

const Contradiction knownContradictionsArray[KNOWN_CONTRADICTIONS_NUMBER] = {
    {.types = {0},
     .types[MaxVertexCountFact] = true,
     .types[MinEdgeCountFact] = true,
     .occurs = &contradiction_type_1_occurs,
     .n_facts = 2,
     .n_params = 2,
     .type_to_param_idx = {{-1}, {0}, {1}, {-1}}},
    {.types = {0},
     .types[MinVertexCountFact] = true,
     .types[MaxVertexCountFact] = true,
     .occurs = &contradiction_type_2_occurs,
     .n_facts = 2,
     .n_params = 2,
     .type_to_param_idx = {{0}, {1}, {-1}, {-1}}},
    {.types = {0},
     .types[MinEdgeCountFact] = true,
     .types[MaxEdgeCountFact] = true,
     .occurs = &contradiction_type_3_occurs,
     .n_facts = 2,
     .n_params = 2,
     .type_to_param_idx = {{-1}, {-1}, {0}, {1}}}};

/**
 * \brief Function which checks if facts from factArray are in one of the predefined contradictions
 * \param factArray array of n_facts Fact objects, fact types should not duplicate, if they do the behaviour is undefined
 * \param n_facts number of facts in array, should be <= MAX_CONTRADICTING_FACTS
 * \return true if facts are in one of the predefined contradictions, false otherwise
 */
bool contradict(Fact **factArray, int n_facts)
{
    if (n_facts > MAX_CONTRADICTING_FACTS)
        return false;
    for (int i = 0; i < KNOWN_CONTRADICTIONS_NUMBER; i++)
    {
        if (n_facts != knownContradictionsArray[i].n_facts)
            continue;
        int *params = (int *)gtd_malloc(knownContradictionsArray[i].n_params * sizeof(int));
        bool fact_types_match = true;
        for (int j = 0; j < n_facts; j++)
        {
            if (!knownContradictionsArray[i].types[factArray[j]->type])
            {
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
            return true;
        gtd_free(params);
    }
    return false;
}