#ifndef GENERATIVE_RESTRICTION_H
#define GENERATIVE_RESTRICTION_H
#include "common.h"
#include "physical_graph.h"

typedef struct GenerativeRestriction GenerativeRestriction;
typedef struct intNode intNode;
typedef struct PathNode PathNode;
typedef struct PathNode
{
    int *path;
    int *pathBool;
    int numElems;
    PathNode *next;
}PathNode;

typedef struct RestrictionResult
{
    int modified;
    int contradictionFound;
}RestrictionResult;

PathNode *find_k_paths(Graph *graph, int k);

GenerativeRestriction *create_restriction(RestrictionResult* (*restriction)(Graph* graph, int *params), 
    int num_params, int *params);
int destroy_restriction(GenerativeRestriction *genRestriction);

RestrictionResult* check_restriction(Graph *graph, GenerativeRestriction *restriction);

GenerativeRestriction *create_max_degree_restriction(int max_vertices);
RestrictionResult* max_degree_restriction_condition(Graph* graph, int *params);

GenerativeRestriction *create_no_k_cycle_restriction(int k);
RestrictionResult* no_k_cycle_restrinction_condition(Graph *graph, int *params);

GenerativeRestriction *create_no_induced_pk_restriction(int k);
RestrictionResult* no_induced_pk_restriction_condition(Graph *graph, int *params);


#endif