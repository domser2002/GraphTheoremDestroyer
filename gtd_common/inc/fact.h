#ifndef FACT_H
#define FACT_H
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "log.h"
#include "common.h"
#include "function.h"
#include "parson.h"
#define FACT_TYPE_NUM 42
#define MAX_PARAMS_IN_FACT 3
#define MAX_FACT_STR_LEN 128
typedef enum FactType
{
    IsConnectedFact = 0,
    IsTreeFact,
    IstnaryTreeFact,
    IsPlanarFact,
    IsPartiteFact,
    IsCycleFact,
    IsCycleComplementFact,
    HasNoCyclesFact,
    VertexCountFact,
    MinVertexCountFact,
    MaxVertexCountFact,
    EdgeCountFact,
    MinEdgeCountFact,
    MaxEdgeCountFact,
    TreeHeightFact,
    MinTreeHeightFact,
    MaxTreeHeightFact,
    HasCycleFact,
    HasNoCycleFact,
    HasInducedCycleFact,
    HasNoInducedCycleFact,
    HasMinorCycleFact,
    HasNoMinorCycleFact,
    HasPathFact,
    HasNoPathFact,
    HasInducedPathFact,
    HasNoInducedPathFact,
    HasMinorPathFact,
    HasNoMinorPathFact,
    HasCompletePartiteFact,
    HasNoCompletePartiteFact,
    HasInducedCompletePartiteFact,
    HasNoInducedCompletePartiteFact,
    HasMinorCompletePartiteFact,
    HasNoMinorCompletePartiteFact,
    HasCliqueFact,
    HasNoCliqueFact,
    HasMinorCliqueFact,
    HasNoMinorCliqueFact,
    MaxVertexDegreeFact,
    MinVertexDegreeFact,
    HasNoUnknownEdgesFact,
    UnknownType = INT32_MAX
} FactType;

/**
 * \brief function to check what parameters are required for fact type
 * \param type fact type
 * \param params pointer to array of strings to return parameter names if it is not NULL
 * \param functional pointer to array of booleans to return if parameter can be functional if it is not NULL
 * \return number of parameters needed
*/
uint8_t get_params(FactType type, char ***params, bool **functional);

/**
 * \brief function to get name of the fact type, to be used by frontend
 */
char *get_fact_type_name(FactType type);

/**
 * \brief function to get fact type by given str name
 * \note it is the reverse of get_fact_type_name
 */
FactType get_fact_type_by_name(const char *name);

typedef struct Fact
{
    FactType type;
    Function **params;
    uint8_t params_count;
} Fact;

Fact *create_fact(FactType type, Function **params);
void delete_fact(Fact *fact);

char *get_fact_str(Fact *fact);

bool equal_facts(Fact *fact1, Fact *fact2);
Fact **deep_copy_fact_array(Fact **fact_array, uint32_t fact_count);

/**
 * \brief function to create a json file with a list of supported restriction/fact types with parameters
 * \param pathname_len length of file name
 * \returns path to the created file
 */
char *create_restrictions_file(size_t *pathname_len);
#endif