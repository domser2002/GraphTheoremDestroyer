#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include "log.h"
#include <stdint.h>
#define GTD_UNUSED(var) (void)var
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
    HasNoMinorCliqueFact,
    MaxVertexDegreeFact,
    MinVertexDegreeFact,
    HasNoUnknownEdgesFact
} FactType;

uint8_t get_param_count(FactType type);

void *gtd_malloc(size_t size);
void *gtd_calloc(size_t nmemb, size_t size);
void *gtd_realloc(void *ptr, size_t size);
void gtd_free(void *ptr);

void printProgressBar(int progress, int total);
#endif