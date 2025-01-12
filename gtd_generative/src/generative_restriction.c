#define IS_GENERATIVE_MODULE_COMPONENT
#include "physical_graph.h"
#include "generative_restriction.h"
#include "generative_proof_machine.h"

struct GenerativeRestriction
{
    RestrictionResult* (*restriction)(Graph* graph, RestrictionParameters *params);
    RestrictionParameters *params;
    FactType type;
    // block = 0 -> restriction can be applied infinitely many times
    // block = 1 -> restriction can be applied only once, but has not been applied yet
    // block = -1 -> restriction can be applied only once, but has already been applied
    int block;
};

typedef struct intNode
{
    int value;
    intNode *next;
}intNode;


GenerativeRestriction *create_restriction_object(RestrictionResult* (*restriction)(Graph* graph, RestrictionParameters *params), RestrictionParameters *params)
{
    GenerativeRestriction *genRestriction = gtd_malloc(sizeof(GenerativeRestriction));
    genRestriction->restriction = restriction;
    genRestriction->params = params;
    genRestriction->block = 0;
    return genRestriction;
}

void set_restriction_block(GenerativeRestriction *genRestriction, int newBlock)
{
    genRestriction->block = newBlock;
}

int get_restriction_block(GenerativeRestriction *genRestriction)
{
    return genRestriction->block;
}

/*
*/

int delete_restriction_object(GenerativeRestriction *genRestriction)
{
    if(genRestriction != NULL)
    {
        gtd_free(genRestriction->params);
        gtd_free(genRestriction);
    }
    return 1;
}

RestrictionResult* validate_restriction(Graph *graph, GenerativeRestriction *restriction)
{
    if(restriction->params->blockRestriction)
    {
        RestrictionResult *res = gtd_malloc(sizeof(RestrictionResult));
        res->contradictionFound = 0;
        res->modified = 0;
        return res;
    }
    RestrictionResult* res = restriction->restriction(graph, restriction->params);
    return res;
}

RestrictionParameters *initialize_restriction_parameters(void)
{
    RestrictionParameters *result = gtd_malloc(sizeof(RestrictionParameters));
    result->numIntParams = 0;
    result->intParams = NULL;
    result->machine = NULL;
    result->blockRestriction = 0;

    return result;
}

void destroy_restriction_parameters(RestrictionParameters *params)
{
    destroy_restriction_parameters_soft(params);
    destroy_generative_proof_machine(params->machine);
}

void destroy_restriction_parameters_soft(RestrictionParameters *params)
{
    gtd_free(params->intParams);
}

RestrictionParameters *deep_copy_restriction_parameters(RestrictionParameters *params)
{
    RestrictionParameters *result = initialize_restriction_parameters();
    result->numIntParams = params->numIntParams;
    result->intParams = gtd_malloc(sizeof(int) * params->numIntParams);
    for(int i = 0; i < params->numIntParams; ++i)
    {
        result->intParams[i] = params->intParams[i];
    }
    result->machine = params->machine;
    result->blockRestriction = params->blockRestriction;
    return result;
}

GenerativeRestriction *deep_copy_restriction(GenerativeRestriction *restriction)
{
    RestrictionParameters *params = deep_copy_restriction_parameters(restriction->params);
    GenerativeRestriction *result = create_restriction_object(restriction->restriction, params);
    set_restriction_block(result, get_restriction_block(restriction));
    return result;
}

RestrictionParameters *get_parameters_from_restriction(GenerativeRestriction *restr)
{
    return restr->params;
}

GenerativeRestriction *create_restriction(FactType restriction_type, RestrictionParameters *params)
{
    RestrictionResult* (*restriction)(Graph* graph, RestrictionParameters *params);
    switch(restriction_type)
    {
        case MaxVertexDegreeFact:
            restriction = max_degree_restriction_condition;
            break;
        case MinVertexDegreeFact:
            restriction = min_degree_condition;
            break;
        case HasNoCycleFact:
            restriction = no_k_cycle_condition;
            break;
        case HasNoInducedPathFact:
            restriction = no_induced_path_k_condition;
            break;
        case HasNoUnknownEdgesFact:
            restriction = edge_check_condition;
            break;
        case HasInducedCycleFact:
            restriction = has_induced_cycle_condition;
            break;
        default:
            restriction = NULL;
    }

    if(restriction == NULL)
    {
        return NULL;
    }

    GenerativeRestriction *result = create_restriction_object(restriction, params);
    result->type = restriction_type;

    switch (restriction_type)
    {
        case HasInducedCycleFact:
            set_restriction_block(result, 1);
            break;
        default:
            set_restriction_block(result, 0);
    }

    return result;
}

GenerativeRestriction *create_restriction_from_fact(Fact *fact, GenerativeProofMachine *machine)
{
    RestrictionParameters *params = (RestrictionParameters*)gtd_malloc(sizeof(RestrictionParameters));
    params->machine = machine;
    params->blockRestriction = 0;
    params->numIntParams = fact->params_count;
    params->intParams = (int*)gtd_malloc(fact->params_count * sizeof(int));
    for(uint32_t i=0;i<fact->params_count;i++)
    {
        if(!is_constant(fact->params[i])) 
        {
            gtd_free(params->intParams);
            return NULL;
        }
        params->intParams[i] = fact->params[i]->c;
    }
    return create_restriction(fact->type, params);
}
// =============== max degree restriction ===============

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

/**
 * \brief constructor for a helper class that stores information about paths
 * \param k integer specyfying a length of a path
 * \param n integer specyfying number of all vertices
 * \returns pointer to the newly create PathNode
 */
PathNode *initPathNode(int k, int n)
{
    PathNode *node = gtd_malloc(sizeof(PathNode));
    node->path = gtd_malloc(sizeof(int) * k);
    
    node->pathBool = gtd_malloc(sizeof(int) * n);
    for(int i = 0; i < n; ++i)
    {
        node->pathBool[i] = 0;
    }

    node->next = NULL;
    node->numElems = 0;
    return node;
}

/**
 * \brief function to destruct PathNode
 * \param pointer to the PathNode to destruct
 */
void destroyPathNode(PathNode *node)
{
    gtd_free(node->path);
    gtd_free(node->pathBool);
    gtd_free(node);
}

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

RestrictionResult* no_k_cycle_condition(Graph *graph, RestrictionParameters *params) 
{
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
                "Sprzecznosc - istnieje cykl C%d: [ ", k);
            for (int i = 0; i < path->numElems; i++) {
                char temp[20];
                snprintf(temp, sizeof(temp), "%d ", path->path[i]);
                strncat(buffer, temp, sizeof(buffer) - strlen(buffer) - 1);
            }
            strncat(buffer, "]", 2);
            ProofNode *proofNode = create_proof_node();
            proofNode->message = strdup(buffer);
            append_proof_node(proofTree, proofNode);

            return result;
        }
        if(adjMatrix[startVertex][endVertex] == UNKNOWN_SYMBOL)
        {
            result->modified = 1;
            set_edge_not_connected(graph, startVertex, endVertex);
            
            // add to proof tree
            char buffer[100];
            snprintf(buffer, sizeof(buffer), 
                "Nie moze powstac krawedz (%d %d), bo powstalby cykl C%d: [ ", 
                startVertex, endVertex, k);
            for (int i = 0; i < path->numElems; i++) {
                char temp[20];
                snprintf(temp, sizeof(temp), "%d ", path->path[i]);
                strncat(buffer, temp, sizeof(buffer) - strlen(buffer) - 1);
            }
            strncat(buffer, "]", 2);
            ProofNode *proofNode = create_proof_node();
            proofNode->message = strdup(buffer);
            append_proof_node(proofTree, proofNode);
        }

        destroyPathNode(path);
    }

    return result;
}

// =============== no k cycle restriction ===============
// =============== no induced pk restriction ============

RestrictionResult* no_induced_path_k_condition(Graph *graph, RestrictionParameters *params) 
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

        // int notConnectedCount = 0;
        int connectedCount = 0;
        int unknownCount = 0;
        int lastUnknownStart = -1;
        int lastUnknownEnd = -1;

        for(int i = 0; i < k; ++i)
        {
            for(int j = i+1; j < k; ++j)
            {
                int iver = path->path[i];
                int jver = path->path[j];
                /*
                if(adjMatrix[iver][jver] == notConnectedCount)
                {
                    ++notConnectedCount;
                }
                */
               if(adjMatrix[iver][jver] == CONNECTED_SYMBOL)
               {
                    ++connectedCount;
               }
                if(adjMatrix[iver][jver] == UNKNOWN_SYMBOL)
                {
                    ++unknownCount;
                    lastUnknownStart = iver;
                    lastUnknownEnd = jver;
                }
            }
        }

        // if(notConnectedCount == (k-1)*(k-2) / 2)
        if(connectedCount == k-1 && unknownCount == 0)
        {

            // add to proof tree
            char buffer[100];
            snprintf(buffer, sizeof(buffer), 
                "Sprzecznosc - istnieje indukowane P%d: [ ", k);
            for (int i = 0; i < path->numElems; i++) {
                char temp[20];
                snprintf(temp, sizeof(temp), "%d ", path->path[i]);
                strncat(buffer, temp, sizeof(buffer) - strlen(buffer) - 1);
            }
            strncat(buffer, "]", 2);
            ProofNode *proofNode = create_proof_node();
            proofNode->message = strdup(buffer);
            append_proof_node(proofTree, proofNode);

            result->contradictionFound = 1;
            destroyPathNode(path);
            while(paths != NULL)
            {
                path = paths;
                paths = paths->next;
                destroyPathNode(path);
            }

            return result;
        }

        // if(unknownCount == 1)
        if(connectedCount == k-1 && unknownCount == 1)
        {
            set_edge_connected(graph, lastUnknownStart, lastUnknownEnd);
            result->modified = 1;

            char buffer[100];
            snprintf(buffer, sizeof(buffer), 
                "Musi istniec krawedz (%d %d), bo powstalaby indukowana P%d: [ ", 
                lastUnknownStart, lastUnknownEnd, k);
            for (int i = 0; i < path->numElems; i++) {
                char temp[20];
                snprintf(temp, sizeof(temp), "%d ", path->path[i]);
                strncat(buffer, temp, sizeof(buffer) - strlen(buffer) - 1);
            }
            strncat(buffer, "]", 2);
            ProofNode *proofNode = create_proof_node();
            proofNode->message = strdup(buffer);
            append_proof_node(proofTree, proofNode);
        }

        destroyPathNode(path);
    }
    return result;
}

// =============== no induced pk restriction ============
// =============== min degree restriction ============

RestrictionResult *min_degree_condition(Graph *graph, RestrictionParameters *params)
{
    RestrictionResult *result = gtd_malloc(sizeof(RestrictionResult));
    result->contradictionFound = 0;
    result->modified = 0;
    int k = params->intParams[0];
    int n = get_graph_num_vertices(graph);
    char **adjMatrix = get_graph_adjacency_matrix(graph);
    ProofTree *proofTree = get_machine_proof_tree(params->machine);

    
    for(int i = 0; i < n; ++i)
    // for(int i = n-1; i >= 0; --i)
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
                ProofNode *proofNode = create_proof_node();
                proofNode->message = strdup(buffer);
                append_proof_node(proofTree, proofNode);

                return result;
            }
        }
    }
    return result;
}

// =============== min degree restriction ============
// =============== check edge restriction ============

RestrictionResult *edge_check_condition(Graph *graph, RestrictionParameters *params)
{
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
    
    for(int i = 0; i < n; ++i)
    {
        for(int j = n-1; j > i; --j)
        {
            if(i == j || adjMatrix[i][j] != UNKNOWN_SYMBOL)
            {
                continue;
            }

            GenerativeProofMachine *originMachine = params->machine;

            GenerativeProofMachine *connMachine = copy_proof_machine(params->machine);
            Graph *connGraph = get_machine_graph(connMachine);
            set_edge_connected(connGraph, i, j);
            set_machine_depth(connMachine, get_machine_depth(connMachine) + 1);
            get_machine_proof_tree(connMachine)->depth = get_machine_depth(connMachine);

            GenerativeProofMachine *notConnMachine = copy_proof_machine(params->machine);
            Graph *notConnGraph = get_machine_graph(notConnMachine);
            set_edge_not_connected(notConnGraph, i, j);
            set_machine_depth(notConnMachine, get_machine_depth(notConnMachine) + 1);
            get_machine_proof_tree(notConnMachine)->depth = get_machine_depth(notConnMachine);

            int contrConn = execute_generative_proof_machine(connMachine);

            int contrNotConn = execute_generative_proof_machine(notConnMachine);

            params->machine = originMachine;

            // todo destroy machines

            if(!contrConn && !contrNotConn)
            {
                continue;
            }

            if(contrConn || contrNotConn)
            {
                result->modified = 1;

                char buffer1[100];
                snprintf(buffer1, sizeof(buffer1), 
                    "Zalozmy, ze krawedz (%d %d) istnieje:", 
                    i, j);
                ProofNode *proofNode1 = create_proof_node();
                proofNode1->message = strdup(buffer1);
                proofNode1->subtree = get_machine_proof_tree(connMachine);
                append_proof_node(get_machine_proof_tree(originMachine), proofNode1);

                char buffer2[100];
                snprintf(buffer2, sizeof(buffer2), 
                    "Zalozmy, ze krawedz (%d %d) nie istnieje:", 
                    i, j);
                ProofNode *proofNode2 = create_proof_node();
                proofNode2->message = strdup(buffer2);
                proofNode2->subtree = get_machine_proof_tree(notConnMachine);
                append_proof_node(get_machine_proof_tree(originMachine), proofNode2);
            }
            if(contrConn && contrNotConn)
            {
                result->contradictionFound = 1;
                return result;
            }

            if(contrConn)
            {
                int depth = get_machine_depth(originMachine);
                load_machine(originMachine, connMachine);
                set_machine_depth(originMachine, depth);
            }
            if(contrNotConn)
            {
                int depth = get_machine_depth(originMachine);
                load_machine(originMachine, notConnMachine);
                set_machine_depth(originMachine, depth);
            }

            if(contrConn || contrNotConn)
            {
                return result;
            }
        }
    }
    return result;
}

// =============== check edge restriction ============
// ========== has induced cycle restriction ==========

RestrictionResult *has_induced_cycle_condition(Graph *graph, RestrictionParameters *params)
{
    RestrictionResult *result = gtd_malloc(sizeof(RestrictionResult));
    ProofTree *proofTree = get_machine_proof_tree(params->machine);
    result->contradictionFound = 0;
    result->modified = 0;
    int k = params->intParams[0];
    if((get_graph_max_vertices(graph) - get_graph_num_vertices(graph) < k) && get_graph_max_vertices(graph) > 0)
    {
        return result;
    }
    int nBefore = get_graph_num_vertices(graph);
    for(int i = 0; i < k; ++i)
    {
        add_vertex(graph);
    }
    for(int i = 0; i < k; ++i)
    {
        for(int j = 0; j < k; ++j)
        {
            if(i == j)
            {
                continue;
            }
            if((i+1) % k == j || (i-1) % k == j)
            {
                set_edge_connected(graph, nBefore + i, nBefore + j);
            }
            else
            {
                set_edge_not_connected(graph, nBefore + i, nBefore + j);
            }
        }
    }
    result->modified = 1;
    char tempBuffer[100] = "[ ";
    for (int i = 0; i < k; i++) {
        char temp[20];
        snprintf(temp, sizeof(temp), "%d ", nBefore + i);
        strncat(tempBuffer, temp, sizeof(tempBuffer) - strlen(tempBuffer) - 1);
    }
    strncat(tempBuffer, "]", sizeof(tempBuffer) - strlen(tempBuffer) - 1);
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%s jest cyklem indukowanym", tempBuffer);

    ProofNode *proofNode = create_proof_node();
    proofNode->message = strdup(buffer);
    append_proof_node(proofTree, proofNode);

    return result;
}

// ========== has induced cycle restriction ==========