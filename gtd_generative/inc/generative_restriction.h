#ifndef GENERATIVE_RESTRICTION_H
#define GENERATIVE_RESTRICTION_H
/** \cond IS_GENERATIVE_MODULE_COMPONENT */
#ifdef IS_GENERATIVE_MODULE_COMPONENT
/** \endcond */

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

/**
 * \brief constructor for RestrictionParameters class
 * \result pointer to the newly created RestrictionParameter
 */
RestrictionParameters *initialize_restriction_parameters(void);
/**
 * \brief function to destruct RestrictionParameters
 * \param params pointer to the RestrictionParameters to destroy
 */
void destroy_restriction_parameters(RestrictionParameters *params);
void destroy_restriction_parameters_soft(RestrictionParameters *params);

/**
 * \brief function to find all paths with length k in a graph
 * \param graph pointer to the Graph, for which paths will be searched
 * \param k integer specyfying length of path
 * \returns pointer to the PathNode that contains all k-paths
 */
PathNode *find_k_paths(Graph *graph, int k);

/**
 * \brief function to destruct GenerativeRestriction without its parameters
 * \param machine pointer to the GenerativeRestriction to destroy
 * \returns 1 if suceeded, otherwise raises fault
 */
int delete_restriction_object(GenerativeRestriction *restriction);

/**
 * \brief function to validate a restriction on a graph
 * \param graph pointer to the PhysicalGraph for which restriction will be validated
 * \param restriction pointer to the GenerativeRestriction that will be validated on a graph
 * \returns pointer to RestrictionResult specifying the result of a restriction validation
 */
RestrictionResult *validate_restriction(Graph *graph, GenerativeRestriction *restriction);

/**
 * \brief constructor for GenerativeRestriction class
 * \param restriction function that applies restriction to a graph
 * \param params pointer to RestrictionParameters that store parameters for restriction
 * \returns pointer to a newly created GenerativeRestriction
 */
GenerativeRestriction *create_restriction_object(RestrictionResult* (*restriction)(Graph* graph, RestrictionParameters *params), RestrictionParameters *params);

/**
 * \brief function to create restriction based on FactType and RestrictionParameters
 * \param restriction_type type of the restriction
 * \param params parameters for the restriction
 * \return newly created restriction of type restriction_type with params parameters
 */
GenerativeRestriction *create_restriction(FactType restriction_type, RestrictionParameters *params);

/**
 * \brief converter from fact to restriction
 * \param fact fact from which restriction is created
 * \param machine machine to whcih restriction will be applied
 * \returns restriction created based on fact, if it has only constant params, NULL otherwise
*/
GenerativeRestriction *create_restriction_from_fact(Fact *fact, GenerativeProofMachine *machine);
void set_restriction_block(GenerativeRestriction *genRestriction, int newBlock);
int get_restriction_block(GenerativeRestriction *genRestriction);

/**
 * \brief function to validate max degree condition
 * \param machine pointer to the GenerativeProofMachine, for which restriction will be validated
 * \returns pointer to the RestrictionResult that will store information about result of restriction validation
 */
RestrictionResult *max_degree_restriction_condition(Graph *graph, RestrictionParameters *params);

/**
 * \brief function to validate no k cycle condition
 * \param machine pointer to the GenerativeProofMachine, for which restriction will be validated
 * \returns pointer to the RestrictionResult that will store information about result of restriction validation
 */
RestrictionResult *no_k_cycle_condition(Graph *graph, RestrictionParameters *params);

/**
 * \brief function to validate no induced k path condition
 * \param machine pointer to the GenerativeProofMachine, for which restriction will be validated
 * \returns pointer to the RestrictionResult that will store information about result of restriction validation
 */
RestrictionResult *no_induced_path_k_condition(Graph *graph, RestrictionParameters *params);

/**
 * \brief function to validate min degree condition
 * \param machine pointer to the GenerativeProofMachine, for which restriction will be validated
 * \returns pointer to the RestrictionResult that will store information about result of restriction validation
 */
RestrictionResult *min_degree_condition(Graph *graph, RestrictionParameters *params);

/**
 * \brief function to validate edge check condition
 * \param machine pointer to the GenerativeProofMachine, for which restriction will be validated
 * \returns pointer to the RestrictionResult that will store information about result of restriction validation
 */
RestrictionResult *edge_check_condition(Graph *graph, RestrictionParameters *params);
RestrictionResult *no_unknown_edges_condition(Graph *graph, RestrictionParameters *params);

/**
 * \brief function to validate has induced cycle condition
 * \param machine pointer to the GenerativeProofMachine, for which restriction will be validated
 * \returns pointer to the RestrictionResult that will store information about result of restriction validation
 */
RestrictionResult *has_induced_cycle_condition(Graph *graph, RestrictionParameters *params);

/**
 * \brief function to make a deep copy of RestrictionParameters object
 * \param params pointer to the RestrictionParameters object
 * \returns deep copy of an object pointed to by params
 */
RestrictionParameters *deep_copy_restriction_parameters(RestrictionParameters *params);

/**
 * \brief function to make a deep copy of GenerativeRestriction object
 * \param restriction pointer to the GenerativeRestriction object
 * \returns deep copy of an object pointed to by restriction
 */
GenerativeRestriction *deep_copy_restriction(GenerativeRestriction *restriction);

/**
 * \brief function to get restriction parameters of a restriction
 * \param restr pointer to the GenerativeRestriction object
 * \return restriction parameters of restr
 */
RestrictionParameters *get_parameters_from_restriction(GenerativeRestriction *restriction);
#endif
#endif
