#include "graph.h"
#include <stdlib.h>

Graph* construct_graph(int vertexCount)
{
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->vertexCount = vertexCount;
    g->edges = (int**)malloc(vertexCount*vertexCount*sizeof(int));
    for(int i=0;i<vertexCount;i++)
    {
        g->edges[i] = (int*)malloc(vertexCount*sizeof(int));
    }
}

void destruct_graph(Graph* g)
{
    for(int i=0;i<g->vertexCount;i++)
    {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}