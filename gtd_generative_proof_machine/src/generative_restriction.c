#include "physical_graph.h"
#include "generative_restriction.h"
#include "generative_proof_machine.h"

struct GenerativeRestriction
{
    RestrictionResult* (*restriction)(Graph* graph, RestrictionParameters *params);
    RestrictionParameters *params;
};

typedef struct intNode
{
    int value;
    intNode *next;
}intNode;



GenerativeRestriction *create_restriction(RestrictionResult* (*restriction)(Graph* graph, RestrictionParameters *params), RestrictionParameters *params)
{
    GenerativeRestriction *genRestriction = gtd_malloc(sizeof(GenerativeRestriction));
    genRestriction->restriction = restriction;
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

RestrictionParameters *initRestrictionParameters(void)
{
    RestrictionParameters *result = gtd_malloc(sizeof(RestrictionParameters));
    result->numInt = 0;
    result->intParams = NULL;
    result->machine = NULL;

    return result;
}

void destroyRestrictionParameters(RestrictionParameters *params)
{
    gtd_free(params->intParams);
    destroy_generative_proof_machine(params->machine);
}

RestrictionParameters *copy_parameters(RestrictionParameters *params)
{
    RestrictionParameters *result = initRestrictionParameters();
    result->numInt = params->numInt;
    result->intParams = gtd_malloc(sizeof(int) * params->numInt);
    for(int i = 0; i < params->numInt; ++i)
    {
        result->intParams[i] = params->intParams[i];
    }
    result->machine = params->machine;
    return result;
}

// 
GenerativeRestriction *copy_restriction(GenerativeRestriction *restriction)
{
    RestrictionParameters *params = copy_parameters(restriction->params);
    GenerativeRestriction *result = create_restriction(restriction->restriction, params);
    return result;
}

RestrictionParameters *get_restriction_parameters(GenerativeRestriction *restr)
{
    return restr->params;
}

// =============== max degree restriction ===============

GenerativeRestriction *create_max_degree_restriction(int max_degree, GenerativeProofMachine *machine)
{
    // int *params = gtd_malloc(sizeof(int));
    // params[0] = max_degree;
    RestrictionParameters *params = initRestrictionParameters();
    params->numInt = 1;
    params->intParams = gtd_malloc(sizeof(int));
    params->intParams[0] = max_degree;
    params->machine = machine;
    GenerativeRestriction *restrinction = create_restriction(max_degree_restriction_condition, params);
    return restrinction;
}

RestrictionResult* max_degree_restriction_condition(Graph* graph, RestrictionParameters *params)
{
    int maxDegree = params->intParams[0];

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

GenerativeRestriction *create_no_k_cycle_restriction(int k, GenerativeProofMachine *machine)
{
    // int *params = gtd_malloc(sizeof(int));
    // params[0] = k;
    RestrictionParameters *params = initRestrictionParameters();
    params->numInt = 1;
    params->intParams = gtd_malloc(sizeof(int));
    params->intParams[0] = k;
    params->machine = machine;
    GenerativeRestriction *restriction = create_restriction(no_k_cycle_restrinction_condition, params);
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
RestrictionResult* no_k_cycle_restrinction_condition(Graph *graph, RestrictionParameters *params) {
    // int k = params[0];
    int k = params->intParams[0];
    PathNode *paths = find_k_paths(graph, k);
    RestrictionResult *result = gtd_malloc(sizeof(RestrictionResult));
    result->contradictionFound = 0;
    result->modified = 0;
    char **adjMatrix = get_graph_adjacency_matrix(graph);
    ProofTree *proofTree = get_machine_proof_tree(params->machine);

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

            // add to proof tree
            char buffer[100];
            snprintf(buffer, sizeof(buffer), 
                "Sprzecznosc - istnieje cykl C%d", k);
            ProofNode *proofNode = initProofNode();
            proofNode->message = strdup(buffer);
            addProofNode(proofTree, proofNode);

            return result;
        }
        if(adjMatrix[startVertex][endVertex] == UNKNOWN_SYMBOL)
        {
            result->modified = 1;
            set_edge_not_connected(graph, startVertex, endVertex);
            
            // add to proof tree
            char buffer[100];
            snprintf(buffer, sizeof(buffer), 
                "Nie moze powstac krawedz (%d %d), bo powstalby cykl C%d", 
                startVertex, endVertex, k);
            ProofNode *proofNode = initProofNode();
            proofNode->message = strdup(buffer);
            addProofNode(proofTree, proofNode);
        }

        destroyPathNode(path);
    }

    return result;
}

// =============== no k cycle restriction ===============
// =============== no induced pk restriction ============

GenerativeRestriction *create_no_induced_pk_restriction(int k, GenerativeProofMachine *machine)
{
    // int *params = gtd_malloc(sizeof(int));
    // params[0] = k;
    RestrictionParameters *params = initRestrictionParameters();
    params->numInt = 1;
    params->intParams = gtd_malloc(sizeof(int));
    params->intParams[0] = k;
    params->machine = machine;
    GenerativeRestriction *restriction = create_restriction(no_induced_pk_restriction_condition, params);
    return restriction;
}


RestrictionResult* no_induced_pk_restriction_condition(Graph *graph, RestrictionParameters *params) 
{
    
    int k = params->intParams[0];
    RestrictionResult *result = gtd_malloc(sizeof(RestrictionResult));
    result->contradictionFound = 0;
    result->modified = 0;

    PathNode *paths = find_k_paths(graph, k);
    char **adjMatrix = get_graph_adjacency_matrix(graph);
    ProofTree *proofTree = get_machine_proof_tree(params->machine);
    while(paths != NULL)
    {
        PathNode *path = paths;
        paths = paths->next;

        int notConnectedCount = 0;
        int unknownCount = 0;
        int lastUnknownStart = -1;
        int lastUnknownEnd = -1;

        for(int i = 0; i < k; ++i)
        {
            for(int j = i+1; j < k; ++j)
            {
                int iver = path->path[i];
                int jver = path->path[j];
                if(adjMatrix[iver][jver] == notConnectedCount)
                {
                    ++notConnectedCount;
                }
                if(adjMatrix[iver][jver] == UNKNOWN_SYMBOL)
                {
                    ++unknownCount;
                    lastUnknownStart = iver;
                    lastUnknownEnd = jver;
                }
            }
        }

        if(notConnectedCount == (k-1)*(k-2) / 2)
        {
            result->contradictionFound = 1;
            destroyPathNode(path);
            while(paths != NULL)
            {
                path = paths;
                paths = paths->next;
                destroyPathNode(path);
            }

            // add to proof tree
            char buffer[100];
            snprintf(buffer, sizeof(buffer), 
                "Sprzecznosc - istnieje indukowane P%d", k);
            ProofNode *proofNode = initProofNode();
            proofNode->message = strdup(buffer);
            addProofNode(proofTree, proofNode);

            return result;
        }

        if(unknownCount == 1)
        {
            set_edge_connected(graph, lastUnknownStart, lastUnknownEnd);
            result->modified = 1;

            char buffer[100];
            snprintf(buffer, sizeof(buffer), 
                "Musi istniec krawedz (%d %d), bo powstalaby indukowana P%d", 
                lastUnknownStart, lastUnknownEnd, k);
            ProofNode *proofNode = initProofNode();
            proofNode->message = strdup(buffer);
            addProofNode(proofTree, proofNode);
        }

        destroyPathNode(path);
    }
    return result;
}



// =============== no induced pk restriction ============
// =============== min degree restriction ============

GenerativeRestriction *create_min_degree_restriction(int k, GenerativeProofMachine *machine)
{
    // int *params = gtd_malloc(sizeof(int));
    // params[0] = k;
    RestrictionParameters *params = initRestrictionParameters();
    params->numInt = 1;
    params->intParams = gtd_malloc(sizeof(int));
    params->intParams[0] = k;
    params->machine = machine;
    GenerativeRestriction *restriction = create_restriction(min_degree_restriction_condition, params);
    return restriction;
}

RestrictionResult *min_degree_restriction_condition(Graph *graph, RestrictionParameters *params)
{
    RestrictionResult *result = gtd_malloc(sizeof(RestrictionResult));
    result->contradictionFound = 0;
    result->modified = 0;
    int k = params->intParams[0];
    int n = get_graph_num_vertices(graph);
    char **adjMatrix = get_graph_adjacency_matrix(graph);
    ProofTree *proofTree = get_machine_proof_tree(params->machine);

    
    for(int i = 0; i < n; ++i)
    {
        int numUnknown = 0;
        int numConnected = 0;
        for(int j = 0; j < n; ++j)
        {
            if(i == j)
            {
                continue;
            }
            if(adjMatrix[i][j] == UNKNOWN_SYMBOL)
            {
                ++numUnknown;
            }
            if(adjMatrix[i][j] == CONNECTED_SYMBOL)
            {
                ++numConnected;
            }
        }
        if(numUnknown + numConnected < k)
        {
            if(add_vertex(graph) == 1)
            {
                set_edge_connected(graph, i, n);
                result->modified = 1;

                char buffer[100];
                snprintf(buffer, sizeof(buffer), 
                    "Wierzcholek %d musi miec nowego sasiada - niech bedzie nim %d", 
                    i, n);
                ProofNode *proofNode = initProofNode();
                proofNode->message = strdup(buffer);
                addProofNode(proofTree, proofNode);

                return result;
            }
        }
    }
    return result;
}

// =============== min degree restriction ============
// =============== check edge restriction ============

GenerativeRestriction *create_check_edge_restriction(int max_depth, GenerativeProofMachine *machine)
{
    // int *params = gtd_malloc(sizeof(int));
    // params[0] = max_depth;
    RestrictionParameters *params = initRestrictionParameters();
    params->numInt = 1;
    params->intParams = gtd_malloc(sizeof(int));
    params->intParams[0] = max_depth;
    params->machine = machine;
    GenerativeRestriction *restriction = create_restriction(check_edge_restriction_condition, params);
    return restriction;
}

RestrictionResult *check_edge_restriction_condition(Graph *graph, RestrictionParameters *params)
{
    // int max_depth = params[0];
    int max_depth = params->intParams[0];
    int n = get_graph_num_vertices(graph);
    char **adjMatrix = get_graph_adjacency_matrix(graph);
    RestrictionResult *result = gtd_malloc(sizeof(RestrictionResult));
    result->contradictionFound = 0;
    result->modified = 0;

    if(get_machine_depth(params->machine) == max_depth)
    {
        return result;
    }

    for(int i = n; i > 0; --i)
    {
        for(int j = n; j > 0; --j)
        {
            if(i == j || adjMatrix[i][j] != UNKNOWN_SYMBOL)
            {
                continue;
            }

            GenerativeProofMachine *originMachine = params->machine;

            GenerativeProofMachine *connMachine = copyMachine(params->machine);
            Graph *connGraph = get_machine_graph(connMachine);
            set_edge_connected(connGraph, i, j);
            set_machine_depth(connMachine, get_machine_depth(connMachine) + 1);

            GenerativeProofMachine *notConnMachine = copyMachine(params->machine);
            Graph *notConnGraph = get_machine_graph(notConnMachine);
            set_edge_not_connected(notConnGraph, i, j);
            set_machine_depth(notConnMachine, get_machine_depth(notConnMachine) + 1);
            
            // params->machine = connMachine;
            int contrConn = execute_generative_proof_machine(connMachine);

            // params->machine = notConnMachine;
            int contrNotConn = execute_generative_proof_machine(notConnMachine);

            params->machine = originMachine;

            // todo destroy machines

            if(!contrConn && !contrNotConn)
            {
                continue;
            }

            Graph *graph = get_machine_graph(params->machine);
            if(contrConn)
            {
                set_edge_not_connected(graph, i, j);
                result->modified = 1;

                ProofNode *proofNode = initProofNode();
                proofNode->subtree = get_machine_proof_tree(connMachine);
                addProofNode(get_machine_proof_tree(originMachine), proofNode);
            }
            if(contrNotConn)
            {
                set_edge_connected(graph, i, i);
                result->modified = 1;

                ProofNode *proofNode = initProofNode();
                proofNode->subtree = get_machine_proof_tree(notConnMachine);
                addProofNode(get_machine_proof_tree(originMachine), proofNode);
            }
            if(contrConn && contrNotConn)
            {
                result->contradictionFound = 1;
                return result;
            }
        }
    }
    return result;
}

// =============== check edge restriction ============