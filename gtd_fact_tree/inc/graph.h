#ifndef GRAPH_H
#define GRAPH_H
#include <stdint.h>
#include <stdbool.h>

typedef struct Graph
{
    uint32_t vertexCount;
    uint32_t **edges;
    uint32_t *parents;
    void **vertexData;
} Graph;

Graph *construct(uint32_t vertexCount);
void destruct(Graph* g);
bool add_vertex_with_edge(Graph *g, uint32_t parent_idx, void *newData);
uint32_t get_parent(Graph* g, uint32_t idx);
#endif