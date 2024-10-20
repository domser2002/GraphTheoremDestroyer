#ifndef GRAPH_H
#define GRAPH_H
#include <stdint.h>
#include <stdbool.h>

typedef struct FactTree
{
    uint32_t vertexCount;
    uint32_t **edges;
    uint32_t *parents;
    void **vertexData;
} FactTree;

FactTree *construct(uint32_t vertexCount);
void destruct(FactTree* g);
bool add_vertex_with_edge(FactTree *g, uint32_t parent_idx, void *newData);
uint32_t get_parent(FactTree* g, uint32_t idx);
#endif