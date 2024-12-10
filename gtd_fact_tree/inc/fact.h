#ifndef FACT_H
#define FACT_H
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "log.h"
#include "common.h"
#include "function.h"

typedef struct Fact Fact;

Fact *create_fact(FactType type, Function **params, int params_count);
void delete_fact(Fact *fact);

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