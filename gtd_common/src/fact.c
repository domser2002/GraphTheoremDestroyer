#include "fact.h"

/**
 * \brief constructor for Fact class
*/
Fact *create_fact(FactType type, Function **params)
{
    uint8_t params_count = get_params(type, NULL, NULL);
    Fact *newFact = (Fact *)gtd_malloc(sizeof(Fact));
    newFact->type = type;
    newFact->params = (params_count == 0)?NULL:(Function**)gtd_malloc(params_count * sizeof(Function*));
    for(uint8_t i=0;i<params_count;i++)
        newFact->params[i] = params[i];
    newFact->params_count = params_count;
    char *fact_str = get_fact_str(newFact);
    GTD_LOG("Created: %s", fact_str);
    free(fact_str);
    return newFact;
}

/**
 * \brief destructor for Fact class
*/
void delete_fact(Fact *fact)
{
    if(fact == NULL) 
        return;
    for(uint8_t i=0; i<fact->params_count;i++)
        delete_function(fact->params[i]);
    gtd_free(fact->params);
    gtd_free(fact);
    return;
}

uint8_t get_params(FactType type, char ***params, bool **functional)
{
    if(params != NULL)
    {
        switch (type)
        {
            case IstnaryTreeFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "t";
                break;
            case HasCycleFact:
            case HasNoCycleFact:
            case HasInducedCycleFact:
            case HasNoInducedCycleFact:
            case HasMinorCycleFact:
            case HasNoMinorCycleFact:
            case HasPathFact:
            case HasNoPathFact:
            case HasInducedPathFact:
            case HasNoInducedPathFact:
            case HasMinorPathFact:
            case HasNoMinorPathFact:
            case HasCliqueFact:
            case HasNoCliqueFact:
            case HasMinorCliqueFact:
            case HasNoMinorCliqueFact:
            case IsPartiteFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "k";
                break;
            case VertexCountFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "vertex count";
                break;
            case MinVertexCountFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "min vertex count";
                break;
            case MaxVertexCountFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "max vertex count";
                break;
            case EdgeCountFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "edge count";
                break;
            case MinEdgeCountFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "min edge count";
                break;
            case MaxEdgeCountFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "max edge count";
                break;
            case TreeHeightFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "tree height";
                break;
            case MinTreeHeightFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "min tree height";
                break;
            case MaxTreeHeightFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "max tree height";
                break;
            case MaxVertexDegreeFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "max degree";
                break;
            case MinVertexDegreeFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "min degree";
                break;
            case HasNoUnknownEdgesFact:
                *params = (char **)gtd_malloc(1 * sizeof(char*));
                (*params)[0] = "max depth";
                break;
            case IsConnectedFact:
            case IsTreeFact:
            case IsPlanarFact:
            case IsCycleFact:
            case IsCycleComplementFact:
            case HasNoCyclesFact:
                *params = NULL;
                break;
            case HasCompletePartiteFact:
            case HasNoCompletePartiteFact:
            case HasInducedCompletePartiteFact:
            case HasNoInducedCompletePartiteFact:
            case HasMinorCompletePartiteFact:
            case HasNoMinorCompletePartiteFact:
                *params = (char **)gtd_malloc(2 * sizeof(char*));
                (*params)[0] = "s";
                (*params)[1] = "t";
                break;
            default:
                *params = NULL;
        }
    }
    if(functional != NULL)
    {
        switch (type)
        {
        case MaxVertexDegreeFact:
        case MinVertexDegreeFact:
        case VertexCountFact:
        case MinVertexCountFact:
        case MaxVertexCountFact:
        case EdgeCountFact:
        case MinEdgeCountFact:
        case MaxEdgeCountFact:
        case TreeHeightFact:
        case MinTreeHeightFact:
        case MaxTreeHeightFact:
            *functional = (bool *)gtd_calloc(1, sizeof(bool));
            *functional[0] = true;
            break;
        case IstnaryTreeFact:
        case IsPartiteFact:
        case HasCycleFact:
        case HasNoCycleFact:
        case HasInducedCycleFact:
        case HasNoInducedCycleFact:
        case HasMinorCycleFact:
        case HasNoMinorCycleFact:
        case HasPathFact:
        case HasNoPathFact:
        case HasInducedPathFact:
        case HasNoInducedPathFact:
        case HasMinorPathFact:
        case HasNoMinorPathFact:
        case HasCliqueFact:
        case HasNoCliqueFact:
        case HasMinorCliqueFact:
        case HasNoMinorCliqueFact:
        case HasNoUnknownEdgesFact:
            *functional = (bool *)gtd_calloc(1, sizeof(bool));
            break;
        case IsConnectedFact:
        case IsTreeFact:
        case IsPlanarFact:
        case IsCycleFact:
        case IsCycleComplementFact:
        case HasNoCyclesFact:
            *functional = NULL;
            break;
        case HasCompletePartiteFact:
        case HasNoCompletePartiteFact:
        case HasInducedCompletePartiteFact:
        case HasNoInducedCompletePartiteFact:
        case HasMinorCompletePartiteFact:
        case HasNoMinorCompletePartiteFact:
            *functional = (bool *)gtd_calloc(3, sizeof(bool));
            break;
        default:
            return 0;
        }
    }
    switch (type)
    {
    case IstnaryTreeFact:
    case IsPartiteFact:
    case VertexCountFact:
    case MinVertexCountFact:
    case MaxVertexCountFact:
    case EdgeCountFact:
    case MinEdgeCountFact:
    case MaxEdgeCountFact:
    case TreeHeightFact:
    case MinTreeHeightFact:
    case MaxTreeHeightFact:
    case HasCycleFact:
    case HasNoCycleFact:
    case HasInducedCycleFact:
    case HasNoInducedCycleFact:
    case HasMinorCycleFact:
    case HasNoMinorCycleFact:
    case HasPathFact:
    case HasNoPathFact:
    case HasInducedPathFact:
    case HasNoInducedPathFact:
    case HasMinorPathFact:
    case HasNoMinorPathFact:
    case HasCliqueFact:
    case HasNoCliqueFact:
    case HasMinorCliqueFact:
    case HasNoMinorCliqueFact:
    case MaxVertexDegreeFact:
    case MinVertexDegreeFact:
    case HasNoUnknownEdgesFact:
        return 1;
    case IsConnectedFact:
    case IsTreeFact:
    case IsPlanarFact:
    case IsCycleFact:
    case IsCycleComplementFact:
    case HasNoCyclesFact:
        return 0;
    case HasCompletePartiteFact:
    case HasNoCompletePartiteFact:
    case HasInducedCompletePartiteFact:
    case HasNoInducedCompletePartiteFact:
    case HasMinorCompletePartiteFact:
    case HasNoMinorCompletePartiteFact:
        // handle only bipartite for now
        return 3;
    default:
        return 0;
    }
}

char *get_fact_type_name(FactType type)
{
    char *result = (char *)gtd_malloc(MAX_FACT_STR_LEN * sizeof(char));
    switch (type)
    {
    case IsConnectedFact:
        sprintf(result, "Is connected");
        return result;
    case IsTreeFact:
        sprintf(result, "Is a tree");
        return result;
    case IstnaryTreeFact:
        sprintf(result, "Is a t-nary tree");
        return result;
    case IsPlanarFact:
        sprintf(result, "Is planar");
        return result;
    case IsPartiteFact:
        sprintf(result, "Is k-partite");
        return result;
    case IsCycleFact:
        sprintf(result, "Is a cycle");
        return result;
    case IsCycleComplementFact:
        sprintf(result, "Is a complement of a cycle");
        return result;
    case HasNoCyclesFact:
        sprintf(result, "Has no cycles");
        return result;
    case VertexCountFact:
        sprintf(result, "Vertex count");
        return result;
    case MinVertexCountFact:
        sprintf(result, "Min vertex count");
        return result;
    case MaxVertexCountFact:
        sprintf(result, "Max vertex count");
        return result;
    case EdgeCountFact:
        sprintf(result, "Edge count");
        return result;
    case MinEdgeCountFact:
        sprintf(result, "Min edge count");
        return result;
    case MaxEdgeCountFact:
        sprintf(result, "Max edge count");
        return result;
    case TreeHeightFact:
        sprintf(result, "Tree height");
        return result;
    case MinTreeHeightFact:
        sprintf(result, "Min tree height");
        return result;
    case MaxTreeHeightFact:
        sprintf(result, "Max tree height");
        return result;
    case HasCycleFact:
        sprintf(result, "Contains C_k");
        return result;   
    case HasNoCycleFact:
        sprintf(result, "Does not contain C_k");
        return result;  
    case HasInducedCycleFact:
        sprintf(result, "Contains induced C_k");
        return result;    
    case HasNoInducedCycleFact:
        sprintf(result, "Does not contain induced C_k");
        return result;  
    case HasMinorCycleFact:
        sprintf(result, "Contains minor C_k");
        return result;      
    case HasNoMinorCycleFact:
        sprintf(result, "Does not contain minor C_k");
        return result;  
    case HasPathFact:
        sprintf(result, "Contains P_k");
        return result;    
    case HasNoPathFact:
        sprintf(result, "Does not contain P_k");
        return result;    
    case HasInducedPathFact:
        sprintf(result, "Contains induced P_k");
        return result;   
    case HasNoInducedPathFact:
        sprintf(result, "Does not contain induced P_k");
        return result; 
    case HasMinorPathFact:
        sprintf(result, "Contains minor P_k");
        return result;   
    case HasNoMinorPathFact:
        sprintf(result, "Does not contain minor P_k");
        return result; 
    case HasCompletePartiteFact:
        sprintf(result, "Contains K_(s,t)");
        return result;  
    case HasNoCompletePartiteFact:
        sprintf(result, "Does not contain K_(s,t)");
        return result; 
    case HasInducedCompletePartiteFact:
        sprintf(result, "Contains induced K_(s,t)");
        return result;  
    case HasNoInducedCompletePartiteFact:
        sprintf(result, "Does not contain induced K_(s,t)");
        return result;  
    case HasMinorCompletePartiteFact:
        sprintf(result, "Contains minor K_(s,t)");
        return result;  
    case HasNoMinorCompletePartiteFact:
        sprintf(result, "Does not contain minor K_(s,t)");
        return result; 
    case HasCliqueFact:
        sprintf(result, "Contains K_k");
        return result;  
    case HasNoCliqueFact:
        sprintf(result, "Does not contain K_k");
        return result; 
    case HasMinorCliqueFact:
        sprintf(result, "Contains minor K_k");
        return result;  
    case HasNoMinorCliqueFact:
        sprintf(result, "Does not contain minor K_k");
        return result; 
    case MaxVertexDegreeFact:
        sprintf(result, "Max vertex degree");
        return result; 
    case MinVertexDegreeFact:
        sprintf(result, "Min vertex degree");
        return result;    
    case HasNoUnknownEdgesFact:
        sprintf(result, "Check cases");
        return result; 
    default:
        return "";
    }
}

FactType get_fact_type_by_name(const char *name)
{
    for(uint32_t i=0; i<FACT_TYPE_NUM; i++)
    {
        if(strcmp(name, get_fact_type_name(i)) == 0)
            return i;
    }
    return UnknownType;
}

/**
 * \brief function to check if 2 facts are equal
 * \param fact1 first fact
 * \param fact2 second fact
 * \return true if facts are equal, false otherwise
*/
bool equal_facts(Fact *fact1, Fact *fact2)
{
    if(fact1->type != fact2->type) return false;
    for(uint8_t i=0;i<fact1->params_count;i++)
    {
        if(compare_functions(fact1->params[i], fact2->params[i]))
            return false;
    }
    return true;
}

/**
 * \brief function to get a string from Fact object
 * \note used for writting the proof
 * \param fact Fact object
 * \return string description of Fact
*/
char *get_fact_str(Fact *fact)
{
    char *result = (char *)gtd_malloc(MAX_FACT_STR_LEN * sizeof(char));
    char *str = NULL;
    switch (fact->type)
    {
    case IsConnectedFact:
        sprintf(result, "Graph is connected");
        return result;
    case IsTreeFact:
        sprintf(result, "Graph is a tree");
        return result;
    case IstnaryTreeFact:
        if(is_equal_constant_function(fact->params[0], 1))
            sprintf(result, "Graph is a path");
        else
        {
            str = get_function_str(fact->params[0]);
            sprintf(result, "Graph is a %s-nary tree", str);
            gtd_free(str);
        }
        return result;
    case IsPlanarFact:
        sprintf(result, "Graph is planar");
        return result;
    case IsPartiteFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph is %s-partite", str);
        gtd_free(str);
        return result;
    case IsCycleFact:
        sprintf(result, "Graph is a cycle");
        return result;
    case IsCycleComplementFact:
        sprintf(result, "Graph is a complement of a cycle");
        return result;
    case HasNoCyclesFact:
        sprintf(result, "Graph has no cycles");
        return result;
    case VertexCountFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph has %s vertices", str);
        gtd_free(str);
        return result;
    case MinVertexCountFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph has at least %s vertices", str);
        gtd_free(str);
        return result;
    case MaxVertexCountFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph has at most %s vertices", str);
        gtd_free(str);
        return result;
    case EdgeCountFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph has %s edges", str);
        gtd_free(str);
        return result;
    case MinEdgeCountFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph has at least %s edges", str);
        gtd_free(str);
        return result;
    case MaxEdgeCountFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph has at most %s edges", str);
        gtd_free(str);
        return result;
    case TreeHeightFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph is a tree with height %s", str);
        gtd_free(str);
        return result;
    case MinTreeHeightFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph is a tree with height at least %s", str);
        gtd_free(str);
        return result;
    case MaxTreeHeightFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph is a tree with height at most %s", str);
        gtd_free(str);
        return result;
    case HasCycleFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph contains C_%s as a subgraph", str);
        gtd_free(str);
        return result;   
    case HasNoCycleFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph does not contain C_%s as a subgraph", str);
        gtd_free(str);
        return result;  
    case HasInducedCycleFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph contains C_%s as an induced subgraph", str);
        gtd_free(str);
        return result;   
    case HasNoInducedCycleFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph does not contain C_%s as an induced subgraph", str);
        gtd_free(str);
        return result;  
    case HasMinorCycleFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph contains C_%s as a minor", str);
        gtd_free(str);
        return result;   
    case HasNoMinorCycleFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph does not contain C_%s as a minor", str);
        gtd_free(str);
        return result; 
    case HasPathFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph contains P_%s as a subgraph", str);
        gtd_free(str);
        return result;   
    case HasNoPathFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph does not contain P_%s as a subgraph", str);
        gtd_free(str);
        return result;  
    case HasInducedPathFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph contains P_%s as an induced subgraph", str);
        gtd_free(str);
        return result;   
    case HasNoInducedPathFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph does not contain P_%s as an induced subgraph", str);
        gtd_free(str);
        return result;  
    case HasMinorPathFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph contains P_%s as a minor", str);
        gtd_free(str);
        return result;   
    case HasNoMinorPathFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph does not contain P_%s as a minor", str);
        gtd_free(str);
        return result;
    case HasCompletePartiteFact:
        sprintf(result, "Graph contains K_");
        for(int32_t i=0;i<fact->params[0]->c;i++)
        {
            str = get_function_str(fact->params[i+1]);
            sprintf(result + strlen(result),"%s", str);
            gtd_free(str);
            if(i != fact->params[0]->c - 1) 
                sprintf(result + strlen(result),",");
        }
        sprintf(result + strlen(result), " as a subgraph");
        return result;   
    case HasNoCompletePartiteFact:
        sprintf(result, "Graph does not contain K_");
        for(int32_t i=0;i<fact->params[0]->c;i++)
        {
            str = get_function_str(fact->params[i+1]);
            sprintf(result + strlen(result),"%s", str);
            gtd_free(str);
            if(i != fact->params[0]->c - 1) 
                sprintf(result + strlen(result),",");
        }
        sprintf(result + strlen(result), " as a subgraph");
        return result;  
    case HasInducedCompletePartiteFact:
        sprintf(result + strlen(result), "Graph contains K_");
        for(int32_t i=0;i<fact->params[0]->c;i++)
        {
            str = get_function_str(fact->params[i+1]);
            sprintf(result + strlen(result),"%s", str);
            gtd_free(str);
            if(i != fact->params[0]->c - 1) 
                sprintf(result + strlen(result),",");
        }
        sprintf(result + strlen(result), " as a induced subgraph");
        return result;   
    case HasNoInducedCompletePartiteFact:
        sprintf(result, "Graph does not contain K_");
        for(int32_t i=0;i<fact->params[0]->c;i++)
        {
            str = get_function_str(fact->params[i+1]);
            sprintf(result + strlen(result),"%s", str);
            gtd_free(str);
            if(i != fact->params[0]->c - 1) 
                sprintf(result + strlen(result),",");
        }
        sprintf(result + strlen(result), " as a induced subgraph");
        return result;  
    case HasMinorCompletePartiteFact:
        sprintf(result, "Graph contains K_");
        for(int32_t i=0;i<fact->params[0]->c;i++)
        {
            str = get_function_str(fact->params[i+1]);
            sprintf(result,"%s", str);
            gtd_free(str);
            if(i != fact->params[0]->c - 1) 
                sprintf(result,",");
        }
        sprintf(result, " as a minor");
        return result;   
    case HasNoMinorCompletePartiteFact:
        sprintf(result, "Graph does not contain K_");
        for(int32_t i=0;i<fact->params[0]->c;i++)
        {
            str = get_function_str(fact->params[i+1]);
            sprintf(result,"%s", str);
            gtd_free(str);
            if(i != fact->params[0]->c - 1) 
                sprintf(result,",");
        }
        sprintf(result, " as a minor");
        return result;
    case HasCliqueFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph contains K_%s as a subgraph", str);
        gtd_free(str);
        return result;
    case HasNoCliqueFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph does not contain K_%s as a subgraph", str);
        gtd_free(str);
        return result;
    case HasMinorCliqueFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph contains K_%s as a minor", str);
        gtd_free(str);
        return result;   
    case HasNoMinorCliqueFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Graph does not contain K_%s as a minor", str);
        gtd_free(str);
        return result; 
    case MaxVertexDegreeFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Max vertex degree in graph is %s", str);
        gtd_free(str);
        return result; 
    case MinVertexDegreeFact:
        str = get_function_str(fact->params[0]);
        sprintf(result, "Min vertex degree in graph is %s", str);
        gtd_free(str);
        return result;    
    case HasNoUnknownEdgesFact:
        sprintf(result, "Graph has no unknown edges");
        return result; 
    default:
        return "";
    }
}

Fact **deep_copy_fact_array(Fact **fact_array, uint32_t fact_count)
{
    Fact **new_fact_array = (Fact **)gtd_malloc(fact_count * sizeof(Fact*));
    for(uint32_t i=0; i<fact_count; i++)
    {
        Function **new_params = (Function **)gtd_malloc(fact_array[i]->params_count * sizeof(Function*));
        for(uint8_t j=0;j<fact_array[i]->params_count; j++)
        {
            new_params[j] = copy_function(fact_array[i]->params[j]);
        }
        new_fact_array[i] = create_fact(fact_array[i]->type, new_params);
    }
    return new_fact_array;
}

char *create_restrictions_file(size_t *pathname_len)
{
    char *pathname = (char *)gtd_malloc(MAX_PATHNAME_LEN * sizeof(char));
    char time_str[20];
    char cwd[MAX_PATHNAME_LEN];
    time_t rawtime;
    struct tm *timeinfo;

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(time_str, sizeof(time_str), "%Y%m%d%H%M%S", timeinfo);
        sprintf(pathname, "%s/metadata/restrictions_%s.json", cwd, time_str);
        *pathname_len = strlen(pathname);
    }
    else
    {
        GTD_LOG("getcwd() error");
        exit(EXIT_FAILURE);
    }

    JSON_Value *root_value = json_value_init_array();
    JSON_Array *root_array = json_value_get_array(root_value);
    for(uint32_t i=0; i < FACT_TYPE_NUM; i++)
    {
        JSON_Value *item_value;
        JSON_Object *item_object;
        JSON_Value *int_params_value;
        JSON_Array *int_params_array;
        JSON_Value *functions_value;
        JSON_Array *functions_array;
        bool any_int = false;
        bool any_functional = false;
        item_value = json_value_init_object();
        item_object = json_value_get_object(item_value);
        json_object_set_number(item_object, "id", i);
        json_object_set_string(item_object, "name", get_fact_type_name(i));
        char **param_names;
        bool *functional;
        uint8_t param_count = get_params(i, &param_names, &functional);
        int_params_value = json_value_init_array();
        int_params_array = json_value_get_array(int_params_value);
        functions_value = json_value_init_array();
        functions_array = json_value_get_array(functions_value);
        for(uint8_t j=0; j<param_count; j++)
        {
            if(functional[j])
            {
                any_functional = true;
                json_array_append_string(functions_array, param_names[j]);
            }
            else
            {
                any_int = true;
                json_array_append_string(int_params_array, param_names[j]);
            }
        }
        if(any_int)
            json_object_set_value(item_object, "int_params", int_params_value);
        if(any_functional)
            json_object_set_value(item_object, "functions", functions_value);
        json_array_append_value(root_array, item_value);
    }
    json_serialize_to_file_pretty(root_value, pathname);

    json_value_free(root_value);
    return pathname;
}
