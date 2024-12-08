#ifndef FACT_H
#define FACT_H
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "log.h"
#include "common.h"
#include "function.h"
#define FACT_TYPE_NUM 39
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
    HasNoMinorCliqueFact
} FactType;

typedef struct Fact Fact;

Fact *create_fact(FactType type, Function **params, int params_count);
void delete_fact(Fact *fact);

uint8_t get_param_count(FactType type);

char *get_fact_str(Fact *fact);

bool equal_facts(Fact *fact1, Fact *fact2);

#ifdef CAN_ACCESS_FACT
struct Fact
{
    FactType type;
    Function **params;
    uint8_t params_count;
};
#endif
#endif