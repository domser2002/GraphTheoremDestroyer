#ifndef FACT_H
#define FACT_H
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "log.h"
#include "common.h"
#define FACT_TYPE_NUM 4
#define MAX_PARAMS_IN_FACT 1
typedef enum FactType
{
    MinVertexCountFact = 0,
    MaxVertexCountFact,
    MinEdgeCountFact,
    MaxEdgeCountFact
} FactType;

typedef struct Fact Fact;

Fact *create_fact(FactType type, int *params, int params_count);
int delete_fact(Fact *fact);

int get_param_count(FactType type);

char *get_fact_str(Fact *fact);

bool equal(Fact *fact1, Fact *fact2);

#ifdef CAN_ACCESS_FACT
struct Fact
{
    FactType type;
    uint32_t *params;
    uint8_t params_count;
};
#endif
#endif