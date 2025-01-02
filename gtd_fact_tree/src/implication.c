#define IS_FACT_TREE_COMPONENT
#include "implication.h"
#define KNOWN_IMPLICATIONS_NUMBER 17
#define MAX_RIGHT_SIDE_FACTS 2

typedef bool (*calc_right_side_params_fun)(Function **, Function **);

typedef struct Implication_Left_Side
{
    bool types[FACT_TYPE_NUM];
    uint32_t n_facts;
    int n_params;
    int type_to_param_idx[FACT_TYPE_NUM][MAX_PARAMS_IN_FACT];
} Implication_Left_Side;

typedef struct Implication_Right_Side
{
    uint32_t n_facts;
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
        {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, 
        {-1}, {-1}
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
bool implication_type_1_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    right_side_params[0] = copy_function(left_side_params[0]);
    right_side_params[1] = copy_function(left_side_params[0]);
    char *str1 = get_function_str(left_side_params[0]);
    char *str2 = get_function_str(right_side_params[0]);
    char *str3 = get_function_str(right_side_params[1]);
    GTD_LOG("Calculated that EdgeCount = %s implies minEdgeCount = %s and maxEdgeCount = %s", 
        str1, str2, str3);
    gtd_free(str1);
    gtd_free(str2);
    gtd_free(str3);
    return true;
}

/**
 * \brief implication type 2 - VertexCount implies MinVertexCount and MaxVertexCount
 * \param left_side_params - 1 element array containing VertexCount
 * \param right_side_params - 2 element array for MinVertexCount and MaxVertexCount
 */
bool implication_type_2_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    right_side_params[0] = copy_function(left_side_params[0]);
    right_side_params[1] = copy_function(left_side_params[0]);
    char *str1 = get_function_str(left_side_params[0]);
    char *str2 = get_function_str(right_side_params[0]);
    char *str3 = get_function_str(right_side_params[1]);
    GTD_LOG("Calculated that VertexCount = %s implies minVertexCount = %s and maxVertexCount = %s",
        str1, str2, str3);
    gtd_free(str1);
    gtd_free(str2);
    gtd_free(str3);
    return true;
}

/**
 * \brief implication type 3 - MinEdgeCount implies MinVertexCount
 * \param left_side_params - 1 element array containing MinEdgeCount
 * \param right_side_params - 1 element array for MinVertexCount
 */
bool implication_type_3_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    if (is_equal_constant_function(left_side_params[0], 0)) {
        right_side_params[0] = create_function(0);
    } else {
        Function *result;
        result = copy_function(left_side_params[0]);
        multiply_constant(result, 8);
        add_constant(result, 1);           
        result = sqrt_function(result);            
        add_constant(result, 1);           
        divide_constant(result, 2);
        right_side_params[0] = copy_function(result);

        delete_function(result);
    }
    char *str1 = get_function_str(left_side_params[0]);
    char *str2 = get_function_str(right_side_params[0]);
    GTD_LOG("Calculated that minEdgeCount = %s implies minVertexCount = %s", str1, str2);
    gtd_free(str1);
    gtd_free(str2);
    return true;
}

/**
 * \brief implication type 4 - MaxVertexCount implies MaxEdgeCount
 * \param left_side_params - 1 element array containing MaxVertexCount
 * \param right_side_params - 1 element array for MaxEdgeCount
 */
bool implication_type_4_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    Function *temp = copy_function(left_side_params[0]);
    subtract_constant(temp, 1);
    right_side_params[0] = multiply_functions(left_side_params[0], temp);
    divide_constant(right_side_params[0], 2);

    delete_function(temp);

    char *str1 = get_function_str(left_side_params[0]);
    char *str2 = get_function_str(right_side_params[0]);
    GTD_LOG("Calculated that maxVertexCount = %s implies maxEdgeCount = %s", str1, str2);
    gtd_free(str1);
    gtd_free(str2);
    return true;
}

/**
 * \brief implication type 5 - is tree implies is connected and has no cycles
 * \param left_side_params - 0 element array
 * \param right_side_params - 0 element array
*/
bool implication_type_5_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
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
bool implication_type_6_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    GTD_UNUSED(left_side_params);
    GTD_UNUSED(right_side_params);
    GTD_LOG("Is connected and has no cycles implies is tree");
    return true;
}

/**
 * \brief implication type 8 - Is tree, has no induced complete partite implies is t-nary tree
 * \param left_side_params - 3 element array, describing complete partite graph
 * \param right_side_params - 1 element array for t
 */
bool implication_type_8_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    Function *two, *one;
    one = create_function(1);
    two = create_function(2);

    int8_t res1 = compare_functions(left_side_params[0], two);
    int8_t res2 = compare_functions(left_side_params[1], one);

    delete_function(two);
    delete_function(one);

    if (res1 != 0 || res2 != 0)
        return false;
    right_side_params[0] = copy_function(left_side_params[2]);
    subtract_constant(right_side_params[0], 2);
    char *str1 = get_function_str(left_side_params[1]);
    char *str2 = get_function_str(left_side_params[2]);
    char *str3 = get_function_str(right_side_params[0]);
    GTD_LOG("Calculated that is tree and contains induced K_{%s,%s} implies is %s-nary tree", 
        str1, str2, str3);
    gtd_free(str1);
    gtd_free(str2);
    gtd_free(str3);
    return true;
}

/**
 * \brief implication type 9 - is tree, has no induced path implies max tree height
 * \param left_side_params - 1 element array, path length 
 * \param right_side_params - 1 element array for max tree height
 */
bool implication_type_9_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    right_side_params[0] = copy_function(left_side_params[0]);
    subtract_constant(right_side_params[0], 1); // max tree height = path length - 1
    char *str1 = get_function_str(left_side_params[0]);
    char *str2 = get_function_str(right_side_params[0]);
    GTD_LOG("Calculated that is tree and contains induced P_{%s} implies max tree height is %s", 
        str1, str2);
    gtd_free(str1);
    gtd_free(str2);
    return true;
}

/**
 * \brief implication type 10 - is t-nary tree with max tree height implies max vertex count
 * \param left_side_params - 2 element array, t and h
 * \param right_side_params - 1 element array for max vertex count
 */
bool implication_type_10_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    if(is_equal_constant_function(left_side_params[0], 1))
    {
        right_side_params[0] = copy_function(left_side_params[1]);
    }
    else
    {
        Function *th_minus_one, *t_minus_one;

        // t^h
        th_minus_one = create_function(1);
        Function *i = NULL;
        i = copy_function(left_side_params[1]);
        for (; !is_equal_constant_function(i, 0); subtract_constant(i, 1))
        {
            th_minus_one = multiply_functions(th_minus_one, left_side_params[0]);
        }

        t_minus_one = copy_function(left_side_params[0]);
        subtract_constant(t_minus_one, 1);                        // t - 1
        subtract_constant(th_minus_one, 1);                       // t^h - 1
        right_side_params[0] = divide_functions(th_minus_one, t_minus_one); // (t^h - 1) / (t - 1)

        delete_function(th_minus_one);
        delete_function(t_minus_one);
    }
    char *str1 = get_function_str(left_side_params[0]);
    char *str2 = get_function_str(left_side_params[1]);
    char *str3 = get_function_str(right_side_params[0]);
    GTD_LOG("Calculated that is %s-nary tree with max height = %s implies max vertex count = %s",
            str1, str2, str3);
    gtd_free(str1);
    gtd_free(str2);
    gtd_free(str3);
    return true;
}

/**
 * \brief implication type 11 - no K_{3,3} and K_{5} as minors implies is planar
 * \param left_side_params - 4 element array, describing complete partite graph and clique size
 * \param right_side_params - 0 element array
 */
bool implication_type_11_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    GTD_UNUSED(right_side_params);

    Function *two, *three, *five;
    two = create_function(2);
    three = create_function(3);
    five = create_function(5);

    int8_t res1 = compare_functions(left_side_params[0], two);
    int8_t res2 = compare_functions(left_side_params[1], three);
    int8_t res3 = compare_functions(left_side_params[2], three);
    int8_t res4 = compare_functions(left_side_params[3], five);

    delete_function(two);
    delete_function(three);
    delete_function(five);

    if (res1 != 0 || res2 == 1 || res3 == 1 || res4 == 1)
        return false;
    char *str1 = get_function_str(left_side_params[1]);
    char *str2 = get_function_str(left_side_params[2]);
    char *str3 = get_function_str(left_side_params[3]);
    GTD_LOG("Checked that does not contain K_{%s,%s} and K_{%s} implies is planar", 
        str1, str2, str3);
    gtd_free(str1);
    gtd_free(str2);
    gtd_free(str3);
    return true;
}

/**
 * \brief implication type 14 - cycle complement graphs with n <= 6 are planar
 * \param left_side_params - 1 element array, max vertex count
 * \param right_side_params - 0 element array
 */
bool implication_type_14_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    GTD_UNUSED(right_side_params);

    Function *six;
    six = create_function(6);

    int8_t res = compare_functions(left_side_params[0], six);

    delete_function(six);

    if (res == 1) // n > 6
        return false;
    char *str = get_function_str(left_side_params[0]);
    GTD_LOG("Checked that cycle complement graph with %s vertices is planar", str);
    gtd_free(str);
    return true;
}

/**
 * \brief implication type 15 - cycle complement graph with n = 7 contains K_{3,3} as minor
 * \param left_side_params - 1 element array, vertex count
 * \param right_side_params - 3 element array, describing complete partite graph
 */
bool implication_type_15_calculate_right_side_params(Function **left_side_params, Function **right_side_params)
{
    if (!is_equal_constant_function(left_side_params[0], 7)) // n != 7
        return false;

    right_side_params[0] = create_function(2);
    right_side_params[1] = create_function(3);
    right_side_params[2] = create_function(3);

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
Fact **implies(Fact **factArray, uint32_t n_facts, int *count)
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
        Function **params = (Function **)gtd_malloc(knownImplicationsArray[i].left_side.n_params * sizeof(Function*));
        bool types[FACT_TYPE_NUM] = {};
        for (uint32_t j = 0; j < n_facts; j++)
        {
            types[factArray[j]->type] = true;
        }
        bool fact_types_match = true;
        for(uint32_t j=0; j<FACT_TYPE_NUM; j++)
        {
            if(knownImplicationsArray[i].left_side.types[j] != types[j])
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
            Function **right_side_params = (Function **)gtd_malloc(knownImplicationsArray[i].right_side.n_params * sizeof(Function *));
            for(uint8_t i=0; i<knownImplicationsArray[i].right_side.n_params; i++)
                right_side_params[i] = (Function*)gtd_malloc(sizeof(Function));
            knownImplicationsArray[i].calculate_params(params,right_side_params);
            *count = knownImplicationsArray[i].right_side.n_facts;
            Fact **right_side_facts = (Fact**)gtd_malloc(*count * sizeof(Fact*));
            int counter = 0;
            for(uint32_t k=0;k<knownImplicationsArray[i].right_side.n_facts;k++)
            {
                FactType type = knownImplicationsArray[i].right_side.types[k];
                int n_params = get_param_count(type);
                Function **fact_params = (Function **)gtd_malloc(n_params * sizeof(Function *));
                for(int s=0;s<n_params;s++)
                {
                    fact_params[s] = right_side_params[counter++];
                }
                right_side_facts[k] = create_fact(type,fact_params);
                gtd_free(fact_params);
            }
            gtd_free(params);
            return right_side_facts;
        }
        gtd_free(params);
    }
    return NULL;
}