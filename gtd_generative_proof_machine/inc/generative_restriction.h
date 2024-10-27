#ifndef GENERATIVE_RESTRICTION_H
#define GENERATIVE_RESTRICTION_H
#include "common.h"
#include "physical_graph.h"

typedef struct GenerativeRestriction GenerativeRestriction;

// repair identation
GenerativeRestriction *create_restriction(int (*restriction)(Graph* graph, int *params), 
    int num_params, int *params);
int destroy_restriction(GenerativeRestriction *genRestriction);

int check_restriction(Graph *graph, GenerativeRestriction *restriction);

GenerativeRestriction *create_max_vertices_restriction(int max_vertices);
int max_vertices_restriction_condition(Graph* graph, int *params);

#endif