#define CAN_ACCESS_FACT
#include "fact.h"

/**
 * \brief constructor for Fact class
*/
Fact *create_fact(FactType type, int *params, int params_count)
{
    if(params_count != get_param_count(type)) 
        return NULL;
    Fact *newFact = (Fact *)gtd_malloc(sizeof(Fact));
    newFact->type = type;
    newFact->params = (params_count == 0)?NULL:gtd_malloc(params_count * sizeof(uint32_t));
    for(int i=0;i<params_count;i++)
        newFact->params[i] = params[i];
    newFact->params_count = params_count;
    return newFact;
}

/**
 * \brief destructor for Fact class
*/
int delete_fact(Fact *fact)
{
    gtd_free(fact->params);
    gtd_free(fact);
    return 0;
}

/**
 * \brief function to check how many parameters are required for fact type
 * \param type fact type
 * \return number of parameters needed
*/
int get_param_count(FactType type)
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
bool equal(Fact *fact1, Fact *fact2)
{
    if(fact1->type != fact2->type) return false;
    for(uint8_t i=0;i<fact1->params_count;i++)
    {
        if(fact1->params[i] != fact2->params[i])
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
    char *result = (char *)gtd_malloc(128 * sizeof(char));
    switch (fact->type)
    {
    case IsConnectedFact:
        sprintf(result, "Graph is connected");
        return result;
    case IsTreeFact:
        sprintf(result, "Graph is a tree");
        return result;
    case IstnaryTreeFact:
        sprintf(result, "Graph is a %d-nary tree", fact->params[0]);
        return result;
    case IsPlanarFact:
        sprintf(result, "Graph is planar");
        return result;
    case IsPartiteFact:
        sprintf(result, "Graph is %d-partite", fact->params[0]);
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
        sprintf(result, "Graph has %d vertices", fact->params[0]);
        return result;
    case MinVertexCountFact:
        sprintf(result, "Graph has at least %d vertices", fact->params[0]);
        return result;
    case MaxVertexCountFact:
        sprintf(result, "Graph has at most %d vertices", fact->params[0]);
        return result;
    case EdgeCountFact:
        sprintf(result, "Graph has %d edges", fact->params[0]);
        return result;
    case MinEdgeCountFact:
        sprintf(result, "Graph has at least %d edges", fact->params[0]);
        return result;
    case MaxEdgeCountFact:
        sprintf(result, "Graph has at most %d edges", fact->params[0]);
        return result;
    case TreeHeightFact:
        sprintf(result, "Graph is a tree with height %d", fact->params[0]);
        return result;
    case MinTreeHeightFact:
        sprintf(result, "Graph is a tree with height at least %d", fact->params[0]);
        return result;
    case MaxTreeHeightFact:
        sprintf(result, "Graph is a tree with height at most %d", fact->params[0]);
        return result;
    case HasCycleFact:
        sprintf(result, "Graph contains C_%d as a subgraph", fact->params[0]);
        return result;   
    case HasNoCycleFact:
        sprintf(result, "Graph does not contain C_%d as a subgraph", fact->params[0]);
        return result;  
    case HasInducedCycleFact:
        sprintf(result, "Graph contains C_%d as an induced subgraph", fact->params[0]);
        return result;   
    case HasNoInducedCycleFact:
        sprintf(result, "Graph does not contain C_%d as an induced subgraph", fact->params[0]);
        return result;  
    case HasMinorCycleFact:
        sprintf(result, "Graph contains C_%d as a minor", fact->params[0]);
        return result;   
    case HasNoMinorCycleFact:
        sprintf(result, "Graph does not contain C_%d as a minor", fact->params[0]);
        return result; 
    case HasPathFact:
        sprintf(result, "Graph contains P_%d as a subgraph", fact->params[0]);
        return result;   
    case HasNoPathFact:
        sprintf(result, "Graph does not contain P_%d as a subgraph", fact->params[0]);
        return result;  
    case HasInducedPathFact:
        sprintf(result, "Graph contains P_%d as an induced subgraph", fact->params[0]);
        return result;   
    case HasNoInducedPathFact:
        sprintf(result, "Graph does not contain P_%d as an induced subgraph", fact->params[0]);
        return result;  
    case HasMinorPathFact:
        sprintf(result, "Graph contains P_%d as a minor", fact->params[0]);
        return result;   
    case HasNoMinorPathFact:
        sprintf(result, "Graph does not contain P_%d as a minor", fact->params[0]);
        return result;
    case HasCompletePartiteFact:
        sprintf(result, "Graph contains K_");
        for(uint32_t i=0;i<fact->params[0];i++)
        {
            sprintf(result,"%d", fact->params[i+1]);
            if(i != fact->params[0] - 1) 
                sprintf(result,",");
        }
        sprintf(result, " as a subgraph");
        return result;   
    case HasNoCompletePartiteFact:
        sprintf(result, "Graph does not contain K_");
        for(uint32_t i=0;i<fact->params[0];i++)
        {
            sprintf(result + strlen(result),"%d", fact->params[i+1]);
            if(i != fact->params[0] - 1) 
                sprintf(result + strlen(result),",");
        }
        sprintf(result + strlen(result), " as a subgraph");
        return result;  
    case HasInducedCompletePartiteFact:
        sprintf(result + strlen(result), "Graph contains K_");
        for(uint32_t i=0;i<fact->params[0];i++)
        {
            sprintf(result + strlen(result),"%d", fact->params[i+1]);
            if(i != fact->params[0] - 1) 
                sprintf(result + strlen(result),",");
        }
        sprintf(result + strlen(result), " as a induced subgraph");
        return result;   
    case HasNoInducedCompletePartiteFact:
        sprintf(result, "Graph does not contain K_");
        for(uint32_t i=0;i<fact->params[0];i++)
        {
            sprintf(result + strlen(result),"%d", fact->params[i+1]);
            if(i != fact->params[0] - 1) 
                sprintf(result + strlen(result),",");
        }
        sprintf(result + strlen(result), " as a induced subgraph");
        return result;  
    case HasMinorCompletePartiteFact:
        sprintf(result, "Graph contains K_");
        for(uint32_t i=0;i<fact->params[0];i++)
        {
            sprintf(result,"%d", fact->params[i+1]);
            if(i != fact->params[0] - 1) 
                sprintf(result,",");
        }
        sprintf(result, " as a minor");
        return result;   
    case HasNoMinorCompletePartiteFact:
        sprintf(result, "Graph does not contain K_");
        for(uint32_t i=0;i<fact->params[0];i++)
        {
            sprintf(result,"%d", fact->params[i+1]);
            if(i != fact->params[0] - 1) 
                sprintf(result,",");
        }
        sprintf(result, " as a minor");
        return result;
    case HasCliqueFact:
        sprintf(result, "Graph contains K_%d as a subgraph", fact->params[0]);
        return result;
    case HasNoCliqueFact:
        sprintf(result, "Graph does not contain K_%d as a subgraph", fact->params[0]);
        return result;
    case HasMinorCliqueFact:
        sprintf(result, "Graph contains K_%d as a minor", fact->params[0]);
        return result;   
    case HasNoMinorCliqueFact:
        sprintf(result, "Graph does not contain K_%d as a minor", fact->params[0]);
        return result;       
    default:
        return "";
    }
}
