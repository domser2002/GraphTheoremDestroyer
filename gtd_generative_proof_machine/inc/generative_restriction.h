#ifndef GENERATIVE_RESTRICTION_H
#define GENERATIVE_RESTRICTION_H
#ifdef IS_GENERATIVE_MODULE_COMPONENT

#include "common.h"
#include "physical_graph.h"
#include "generative_proof_machine.h"
#include "fact.h"

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
    int blockRestriction;
} RestrictionParameters;

RestrictionParameters *initialize_restriction_parameters(void);
void destroy_restriction_parameters(RestrictionParameters *params);
void destroy_restriction_parameters_soft(RestrictionParameters *params);
PathNode *find_k_paths(Graph *graph, int k);
int delete_restriction_object(GenerativeRestriction *restriction);
RestrictionResult *validate_restriction(Graph *graph, GenerativeRestriction *restriction);

GenerativeRestriction *create_restriction_object(RestrictionResult* (*restriction)(Graph* graph, RestrictionParameters *params), RestrictionParameters *params);
GenerativeRestriction *create_restriction(FactType restriction_type, RestrictionParameters *params);
GenerativeRestriction *create_restriction_from_fact(Fact *fact, GenerativeProofMachine *machine);
void set_restriction_block(GenerativeRestriction *genRestriction, int newBlock);
int get_restriction_block(GenerativeRestriction *genRestriction);

RestrictionResult *max_degree_restriction_condition(Graph *graph, RestrictionParameters *params);
RestrictionResult *no_k_cycle_condition(Graph *graph, RestrictionParameters *params);
RestrictionResult *no_induced_path_k_condition(Graph *graph, RestrictionParameters *params);
RestrictionResult *min_degree_condition(Graph *graph, RestrictionParameters *params);
RestrictionResult *edge_check_condition(Graph *graph, RestrictionParameters *params);
RestrictionResult *no_unknown_edges_condition(Graph *graph, RestrictionParameters *params);
RestrictionResult *has_induced_cycle_condition(Graph *graph, RestrictionParameters *params);

RestrictionParameters *deep_copy_restriction_parameters(RestrictionParameters *params);
GenerativeRestriction *deep_copy_restriction(GenerativeRestriction *restriction);

RestrictionParameters *get_parameters_from_restriction(GenerativeRestriction *restriction);
#endif
#endif
