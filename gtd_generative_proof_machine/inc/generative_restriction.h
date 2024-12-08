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
} PathNode;

typedef struct RestrictionResult
{
    uint8_t modified;
    uint8_t contradictionFound;
} RestrictionResult;

typedef struct RestrictionParameters
{
    int numIntParams;
    int *intParams;
    GenerativeProofMachine *machine;
} RestrictionParameters;

RestrictionParameters *initialize_restriction_parameters(void);
void destroy_restriction_parameters(RestrictionParameters *params);
PathNode *find_k_paths(Graph *graph, int k);
GenerativeRestriction *create_restriction_object(RestrictionResult *(*condition)(Graph *graph, RestrictionParameters *params), RestrictionParameters *params);
int delete_restriction_object(GenerativeRestriction *restriction);
RestrictionResult *validate_restriction(Graph *graph, GenerativeRestriction *restriction);

GenerativeRestriction *create_max_degree_restriction(int maxVertices, GenerativeProofMachine *machine);
RestrictionResult *max_degree_restriction_condition(Graph *graph, RestrictionParameters *params);

GenerativeRestriction *create_no_k_cycle_restriction(int k, GenerativeProofMachine *machine);
RestrictionResult *no_k_cycle_condition(Graph *graph, RestrictionParameters *params);

GenerativeRestriction *create_no_induced_path_k_restriction(int k, GenerativeProofMachine *machine);
RestrictionResult *no_induced_path_k_condition(Graph *graph, RestrictionParameters *params);

GenerativeRestriction *create_min_degree_restriction(int minDegree, GenerativeProofMachine *machine);
RestrictionResult *min_degree_condition(Graph *graph, RestrictionParameters *params);

GenerativeRestriction *create_edge_check_restriction(int maxDepth, GenerativeProofMachine *machine);
RestrictionResult *edge_check_condition(Graph *graph, RestrictionParameters *params);

RestrictionParameters *deep_copy_restriction_parameters(RestrictionParameters *params);
GenerativeRestriction *deep_copy_restriction(GenerativeRestriction *restriction);

RestrictionParameters *get_parameters_from_restriction(GenerativeRestriction *restriction);

#endif