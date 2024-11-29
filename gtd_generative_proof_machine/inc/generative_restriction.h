#ifndef GENERATIVE_RESTRICTION_H
#define GENERATIVE_RESTRICTION_H
#include "common.h"
#include "physical_graph.h"
#include "generative_proof_machine.h"

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

typedef struct RestrictionParameters
{
    int numInt;
    int *intParams;
    GenerativeProofMachine *machine;
}RestrictionParameters;

RestrictionParameters *initRestrictionParameters(void);
void destroyRestrictionParameters(RestrictionParameters *params);

PathNode *find_k_paths(Graph *graph, int k);

GenerativeRestriction *create_restriction(RestrictionResult* (*restriction)(Graph* graph, RestrictionParameters *params), 
    RestrictionParameters *params);
int destroy_restriction(GenerativeRestriction *genRestriction);

RestrictionResult* check_restriction(Graph *graph, GenerativeRestriction *restriction);

GenerativeRestriction *create_max_degree_restriction(int max_vertices, GenerativeProofMachine *machine);
RestrictionResult* max_degree_restriction_condition(Graph* graph, RestrictionParameters *params);

GenerativeRestriction *create_no_k_cycle_restriction(int k, GenerativeProofMachine *machine);
RestrictionResult* no_k_cycle_restrinction_condition(Graph *graph, RestrictionParameters *params);

GenerativeRestriction *create_no_induced_pk_restriction(int k, GenerativeProofMachine *machine);
RestrictionResult* no_induced_pk_restriction_condition(Graph *graph, RestrictionParameters *params);

GenerativeRestriction *create_min_degree_restriction(int k, GenerativeProofMachine *machine);
RestrictionResult* min_degree_restriction_condition(Graph *Graph, RestrictionParameters *params);

GenerativeRestriction *create_check_edge_restriction(int max_depth, GenerativeProofMachine *machine);
RestrictionResult* check_edge_restriction_condition(Graph *graph, RestrictionParameters *params);

// makes deep copy of everything except of the machine
RestrictionParameters *copy_parameters(RestrictionParameters *params);

// makes deep copy of everything except of the machine
GenerativeRestriction *copy_restriction(GenerativeRestriction *restriction);

RestrictionParameters *get_restriction_parameters(GenerativeRestriction *restr);

#endif