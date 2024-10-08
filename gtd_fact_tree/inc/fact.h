#ifndef FACT_H
#define FACT_H
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "log.h"
#include "common.h"

typedef enum FactType
{
    MinVertexCountFact = 0,
    MaxVertexCountFact,
    MinEdgeCountFact,
    MaxEdgeCountFact
} FactType;

typedef struct Fact Fact;

Fact *create_min_vertex_count_fact(int minVertexCount);
Fact *create_max_vertex_count_fact(int maxVertexCount);
Fact *create_min_edge_count_fact(int minEdgeCount);
Fact *create_max_edge_count_fact(int maxEdgeCount);

int delete_min_vertex_count_fact(Fact *fact);
int delete_max_vertex_count_fact(Fact *fact);
int delete_min_edge_count_fact(Fact *fact);
int delete_max_edge_count_fact(Fact *fact);

bool contradict(Fact *fact1, Fact *fact2);
Fact *results(Fact *fact);

char *get_fact_str(Fact *fact);

#endif