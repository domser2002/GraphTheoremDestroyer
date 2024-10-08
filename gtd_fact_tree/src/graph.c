#include "graph.h"
#include "common.h"
#include <stdlib.h>

Graph *construct(uint32_t vertexCount)
{
    Graph* g = (Graph*)gtd_malloc(sizeof(Graph));
    g->vertexCount = vertexCount;
    g->edges = (uint32_t**)gtd_malloc(vertexCount*vertexCount*sizeof(uint32_t));
    g->vertexData = NULL;
    g->parents = (uint32_t*)gtd_malloc(vertexCount*sizeof(uint32_t));
    for(uint32_t i=0;i<vertexCount;i++)
    {
        g->edges[i] = (uint32_t*)gtd_calloc(vertexCount, sizeof(uint32_t));
        g->parents[i] = -1;
    }
    return g;
}

void destruct(Graph* g)
{
    for(uint32_t i=0;i<g->vertexCount;i++)
    {
        gtd_free(g->edges[i]);
    }
    gtd_free(g->parents);
    gtd_free(g->edges);
    gtd_free(g);
}

static bool exists(Graph *g,uint32_t parent_idx)
{
    for(uint32_t i=0;i<g->vertexCount;i++)
    {
        if(g->parents[i] == parent_idx)
            return false;
    }
    return true;
}

bool add_vertex_with_edge(Graph *g, uint32_t parent_idx, void *newData)
{
    if(exists(g,parent_idx)) return false;
    g->vertexCount++;
    g->vertexData = (void**)gtd_realloc(g->vertexData,g->vertexCount * sizeof(void*));
    g->vertexData[g->vertexCount-1] = newData;
    g->edges = (uint32_t**)gtd_realloc(g->edges,g->vertexCount*g->vertexCount*sizeof(uint32_t));
    g->parents = (uint32_t*)gtd_realloc(g->parents,g->vertexCount*sizeof(uint32_t));
    for(uint32_t i=0;i<g->vertexCount-1;i++)
    {
        g->edges[i] = (uint32_t*)gtd_realloc(g->edges[i],g->vertexCount*sizeof(uint32_t));
    }
    g->edges[g->vertexCount-1] = (uint32_t*)gtd_calloc(g->vertexCount,sizeof(uint32_t));
    g->edges[parent_idx][g->vertexCount-1] = 1;
    g->edges[g->vertexCount-1][parent_idx] = 1;
    g->parents[g->vertexCount-1] = parent_idx;
    return true;
}

uint32_t get_parent(Graph* g, uint32_t idx)
{
    return g->parents[idx];
}
