#include "physical_graph.h"
#include "generative_restriction.h"

struct GenerativeRestriction
{
    RestrictionResult* (*restriction)(Graph* graph, int *params);
    int num_params;
    int *params;
};

typedef struct intNode
{
    int value;
    intNode *next;
}intNode;



GenerativeRestriction *create_restriction(RestrictionResult* (*restriction)(Graph* graph, int *params), int num_params, int *params)
{
    GenerativeRestriction *genRestriction = gtd_malloc(sizeof(GenerativeRestriction));
    genRestriction->restriction = restriction;
    genRestriction->num_params = num_params;
    genRestriction->params = params;
    return genRestriction;
}

int destroy_restriction(GenerativeRestriction *genRestriction)
{
    if(genRestriction != NULL)
    {
        gtd_free(genRestriction->params);
        gtd_free(genRestriction);
    }
    return 1;
}

RestrictionResult* check_restriction(Graph *graph, GenerativeRestriction *restriction)
{
    RestrictionResult* res = restriction->restriction(graph, restriction->params);
    return res;
}

// =============== max degree restriction ===============

GenerativeRestriction *create_max_degree_restriction(int max_degree)
{
    int *params = gtd_malloc(sizeof(int));
    params[0] = max_degree;
    GenerativeRestriction *restrinction = create_restriction(max_degree_restriction_condition, 1, params);
    return restrinction;
}

RestrictionResult* max_degree_restriction_condition(Graph* graph, int *params)
{
    int maxDegree = params[0];

    RestrictionResult *result = gtd_malloc(sizeof(RestrictionResult));
    result->modified = 0;
    result->contradictionFound = 0;

    int n = get_graph_num_vertices(graph);
    int *degree = get_graph_degree(graph);
    char **adjMatrix = get_graph_adjacency_matrix(graph);
    for(int i = 0; i < n; ++i)
    {
        if(degree[i] > maxDegree)
        {
            result->contradictionFound = 1;
            return result;
        }
        if(degree[i] == maxDegree)
        {
            for(int j = 0; j < n; ++j)
            {
                if(i == j)
                {
                    continue;
                }
                if(adjMatrix[i][j] == UNKNOWN_SYMBOL)
                {
                    set_edge_not_connected(graph, i, j);
                    result->modified = 1;
                }
                adjMatrix = get_graph_adjacency_matrix(graph);
            }

        }
    }

    return result;
}

// =============== max degree restriction ===============
// =============== no k cycle restriction ===============

GenerativeRestriction *create_no_k_cycle_restriction(int k)
{
    int *params = gtd_malloc(sizeof(int));
    params[0] = k;
    GenerativeRestriction *restriction = create_restriction(no_k_cycle_restrinction_condition, 1, params);
    return restriction;
}



PathNode *initPathNode(int k, int n)
{
    PathNode *node = gtd_malloc(sizeof(PathNode));
    node->path = gtd_malloc(sizeof(int) * k);
    /*
    for(int i = 0; i < k; ++i)
    {
        node->path[i] = 0;
    }
    */
    node->pathBool = gtd_malloc(sizeof(int) * n);
    for(int i = 0; i < n; ++i)
    {
        node->pathBool[i] = 0;
    }
    /*
    */
    node->next = NULL;
    node->numElems = 0;
    return node;
}

void destroyPathNode(PathNode *node)
{
    gtd_free(node->path);
    gtd_free(node->pathBool);
    gtd_free(node);
}

// find all k-paths
PathNode* find_k_paths(Graph *graph, int k)
{
    PathNode *nodes = NULL;
    int n = get_graph_num_vertices(graph);
    for(int i = 0; i < n; ++i)
    {
        PathNode *node = initPathNode(k, n);
        node->numElems = 1;
        node->path[0] = i;
        node->pathBool[i] = 1;
        if(nodes == NULL)
        {
            nodes = node;
        }
        else
        {
            node->next = nodes;
            nodes = node;
        }
    }

    PathNode *result = NULL;
    char **adjMatrix = get_graph_adjacency_matrix(graph);
    while(nodes != NULL)
    {
        PathNode *node = nodes;
        nodes = nodes->next;

        int lastVertex = node->path[node->numElems - 1];

        for(int i = 0; i < n; ++i)
        {
            if(node->pathBool[i] || adjMatrix[lastVertex][i] != CONNECTED_SYMBOL)
            {
                continue;
            }

            PathNode *newNode = initPathNode(k, n);
            newNode->numElems = node->numElems + 1;
            for(int i = 0; i < node->numElems; ++i)
            {
                newNode->path[i] = node->path[i];
            }
            for(int i = 0; i < n; ++i)
            {
                newNode->pathBool[i] = node->pathBool[i];
            }
            newNode->path[newNode->numElems - 1] = i;
            newNode->pathBool[i] = 1;

            if(newNode->numElems == k)
            {
                if(result == NULL)
                {
                    result = newNode;
                }
                else
                {
                    newNode->next = result;
                    result = newNode;
                }
            }
            else
            {
                newNode->next = nodes;
                nodes = newNode;
            }
        }
        destroyPathNode(node);
    }

    return result;
}

// Main function for no-k-cycle restriction condition
RestrictionResult* no_k_cycle_restrinction_condition(Graph *graph, int *params) {
    int k = params[0];
    PathNode *paths = find_k_paths(graph, k);
    RestrictionResult *result = gtd_malloc(sizeof(RestrictionResult));
    result->contradictionFound = 0;
    result->modified = 0;
    char **adjMatrix = get_graph_adjacency_matrix(graph);

    while(paths != NULL)
    {
        PathNode *path = paths;
        paths = paths->next;

        int startVertex = path->path[0];
        int endVertex = path->path[k-1];

        if(adjMatrix[startVertex][endVertex] == CONNECTED_SYMBOL)
        {
            result->contradictionFound = 1;
            while(paths != NULL)
            {
                PathNode *prev = paths;
                paths = paths->next;
                destroyPathNode(prev);
            }
            return result;
        }
        if(adjMatrix[startVertex][endVertex] == UNKNOWN_SYMBOL)
        {
            result->modified = 1;
            set_edge_not_connected(graph, startVertex, endVertex);
        }

        destroyPathNode(path);
    }

    return result;
}

// =============== no k cycle restriction ===============
// =============== no induced pk restriction ============

GenerativeRestriction *create_no_induced_pk_restriction(int k)
{
    int *params = gtd_malloc(sizeof(int));
    params[0] = k;
    GenerativeRestriction *restriction = create_restriction(no_induced_pk_restriction_condition, 1, params);
    return restriction;
}

// todo
RestrictionResult* no_induced_pk_restriction_condition(Graph *graph, int *params) {
    gtd_free(graph);
    gtd_free(params);
    return NULL;
    /*
    int k = params[0];  // Target path length P_k
    int n = get_graph_num_vertices(graph);

    RestrictionResult *result = gtd_malloc(sizeof(RestrictionResult));
    result->contradictionFound = 0;
    result->modified = 0;

    PathNode *paths = find_k_paths(graph, k);
    char **adjMatrix = get_graph_adjacency_matrix(graph);
    while(paths != NULL)
    {
        PathNode *path = paths;
        paths = paths->next;

        // check if path is induced
        int numUnknown = 1;
        int unknownStart;
        int unknownEnd;
        for(int i = 0; i < path->numElems; ++i)
        {
            for(int j = 0; j < path->numElems; ++j)
            {
                if((j == i+1 || j == i-1) && adjMatrix[i][j] != CONNECTED_SYMBOL)
                {
                    // induced = 0;
                    break;
                }
                else if(adjMatrix[i][j] != NOT_CONNECTED_SYMBOL)
                {
                    // induced = 0;
                    break;
                }

            }
            if(induced == 0)
            {
                break;
            }
        }
        if(!induced)
        {
            continue;
        }


    }
    return result;
    */
}



// =============== no induced pk restriction ============
