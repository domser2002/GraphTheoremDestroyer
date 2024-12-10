#define CAN_ACCESS_FACT
#include "fact.h"

/**
 * \brief constructor for Fact class
*/
Fact *create_fact(FactType type, Function **params, int params_count)
{
    if(params_count != get_param_count(type)) 
        return NULL;
    Fact *newFact = (Fact *)gtd_malloc(sizeof(Fact));
    newFact->type = type;
    newFact->params = (params_count == 0)?NULL:gtd_malloc(params_count * sizeof(Function*));
    for(int i=0;i<params_count;i++)
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

/**
 * \brief function to check how many parameters are required for fact type
 * \param type fact type
 * \return number of parameters needed
*/
uint8_t get_param_count(FactType type)
{
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

/**
 * \brief function to check if 2 facts are equal
 * \note needed because Fact fields are protected
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
    default:
        return "";
    }
}
