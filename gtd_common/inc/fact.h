#ifndef FACT_H
#define FACT_H
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "log.h"
#include "common.h"
#include "function.h"
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
    HasNoUnknownEdgesFact
} FactType;

uint8_t get_param_count(FactType type);
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

#endif