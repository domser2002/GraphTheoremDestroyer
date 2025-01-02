#define IS_FACT_TREE_COMPONENT
#include "contradiction.h"
#define KNOWN_CONTRADICTIONS_NUMBER 9

typedef bool (*contradiction_occurs_fun)(Function **);

typedef struct Contradiction
{
    bool types[FACT_TYPE_NUM];
    contradiction_occurs_fun occurs;
    uint32_t n_facts;
    uint8_t n_params;
    uint8_t type_to_param_idx[FACT_TYPE_NUM][MAX_PARAMS_IN_FACT];
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
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
    {-1}, {-1}
    }
};


/**
 * \brief contradiction type 1 - contradiction between max vertex count and min edge count
 * \param params - array of 2 integers, max vertex count and min edge count
 * \return true if contradiction occurs, false otherwise
 */
static bool contradiction_type_1_occurs(Function **params)
{
    char *str1 = get_function_str(params[0]);
    char *str2 = get_function_str(params[1]);
    GTD_LOG("Checking for contradiction type 1, maxVertexCount = %s, minEdgeCount = %s", str1,str2);
    gtd_free(str1);
    gtd_free(str2);
    Function *result = copy_function(params[0]);
    subtract_constant(result, 1);
    result = multiply_functions(params[0],result);
    divide_constant(result,2);
    int8_t res = compare_functions(result,params[1]);
    delete_function(result);
    return res == -1;
}

/**
 * \brief contradiction type 2 - contradiction between min vertex count and max vertex count
 * \param params - array of 2 integers, min vertex count and max vertex count
 * \return true if contradiction occurs, false otherwise
 */
static bool contradiction_type_2_occurs(Function **params)
{
    char *str1 = get_function_str(params[0]);
    char *str2 = get_function_str(params[1]);
    GTD_LOG("Checking for contradiction type 2, minVertexCount = %s, maxVertexCount = %s", str1, str2);
    gtd_free(str1);
    gtd_free(str2);
    return compare_functions(params[0], params[1]) == 1;
}

/**
 * \brief contradiction type 3 - contradiction between min edge count and max edge count
 * \param params - array of 2 integers, min edge count and max edge count
 * \return true if contradiction occurs, false otherwise
 */
static bool contradiction_type_3_occurs(Function **params)
{
    char *str1 = get_function_str(params[0]);
    char *str2 = get_function_str(params[1]);
    GTD_LOG("Checking for contradiction type 2, minEdgeCount = %s, maxEdgeCount = %s", str1, str2);
    gtd_free(str1);
    gtd_free(str2);
    return compare_functions(params[0], params[1]) == 1;
}

/**
 * \brief contradiction type 5 - contradiction between planar graphs and K_{3,3} as a minor
 * \param params - array of 3 integers, number of partition classes (only 2 is supoorted) and their count 
 * \return true if contradiction occurs (minor actually is K_{3,3} or bigger), false otherwise
 */
static bool contradiction_type_5_occurs(Function **params) {
    char *str1 = get_function_str(params[0]);
    char *str2 = get_function_str(params[1]);
    char *str3 = get_function_str(params[2]);
    GTD_LOG("Checking for contradiction type 5, partitionClassesNum = %s, count1 = %s, count2 = %s", str1, str2, str3);
    gtd_free(str1);
    gtd_free(str2);
    gtd_free(str3);
    Function *two;
    two = create_function(2); 
    int8_t res1 = compare_functions(params[0], two);
    int8_t res2 = compare_functions(params[1], two);
    int8_t res3 = compare_functions(params[2], two);
    delete_function(two);
    return res1 == 0 && res2 == 1 && res3 == 1;
}

/**
 * \brief contradiction type 6 - contradiction between planar graphs and K_{5} as a minor
 * \param params - array of 1 integer, clique size
 * \return true if contradiction occurs (minor actually is K_{5} or bigger), false otherwise
*/
static bool contradiction_type_6_occurs(Function **params)
{
    char *str = get_function_str(params[0]);
    GTD_LOG("Checking for contradiction type 6, clique size = %s", str);
    gtd_free(str);
    Function *four;
    four = create_function(4);
    int8_t res = compare_functions(params[0],four);
    delete_function(four);
    return res == 1;
}

/**
 * \brief contradiction type 7 - contradiction between graph having a cycle and having no cycles
 * \param params - array of 1 integer, cycle size, unused
 * \return always true
*/
static bool contradiction_type_7_occurs(Function **params)
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
static bool contradiction_type_9_occurs(Function **params)
{
    char *str1 = get_function_str(params[0]);
    char *str2 = get_function_str(params[1]);
    GTD_LOG("Checking for contradiction type 9, %s-partite graph has a cycle with length %s", str1, str2);
    gtd_free(str1);
    gtd_free(str2);
    Function *two;
    two = create_function(2);
    int8_t res = compare_functions(params[0], two);
    return res == 0 && mod_function(params[1], 2) == 1;
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
bool contradict(Fact **factArray, uint32_t n_facts)
{
    GTD_LOG("Checking if array of %d facts forms a known contradiction",n_facts);
    if (n_facts > MAX_CONTRADICTING_FACTS)
    {
        GTD_LOG("No contradiction - too many facts");
        return false;
    }
    if(n_facts < MIN_CONTRADICTING_FACTS)
    {
        GTD_LOG("No contradiction - not enough facts");
        return false;
    }
    for (uint32_t i = 0; i < KNOWN_CONTRADICTIONS_NUMBER; i++)
    {
        GTD_LOG("Checking contradiction number %d", i+1);
        if (n_facts != knownContradictionsArray[i].n_facts)
        {
            GTD_LOG("Number of facts does not match");
            continue;
        }
        Function **params = (Function **)gtd_malloc(knownContradictionsArray[i].n_params * sizeof(Function*));
        bool types[FACT_TYPE_NUM] = {};
        for (uint32_t j = 0; j < n_facts; j++)
        {
            types[factArray[j]->type] = true;
        }
        bool fact_types_match = true;
        for(uint32_t j=0; j<FACT_TYPE_NUM; j++)
        {
            if(knownContradictionsArray[i].types[j] != types[j])
            {
                GTD_LOG("Types of facts do not match");
                fact_types_match = false;
                break;
            }
        }
        if(!fact_types_match)
        {
            gtd_free(params);
            continue;
        }
        for (uint32_t j = 0; j < n_facts; j++)
        {
            // fill params array at the correct positions
            uint8_t param_idxs[MAX_PARAMS_IN_FACT];
            for (uint8_t k = 0; k < MAX_PARAMS_IN_FACT; k++)
            {
                param_idxs[k] = knownContradictionsArray[i].type_to_param_idx[factArray[j]->type][k];
            }
            for (uint8_t k = 0; k < factArray[j]->params_count; k++)
            {
                params[param_idxs[k]] = factArray[j]->params[k];
            }
        }
        if (knownContradictionsArray[i].occurs(params))
        {
            GTD_LOG("Contradiction occurs");
            return true;
        }
        gtd_free(params);
    }
    return false;
}