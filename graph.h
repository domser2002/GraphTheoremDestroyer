typedef struct Graph
{
    int vertexCount;
    int **edges; // matrix of neighbourship
};

Graph* construct_graph(int vertexCount);
void destruct_graph(Graph* g);