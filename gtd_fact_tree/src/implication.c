#include "implication.h"

Fact *results(Fact *fact)
{
    switch (fact->type)
    {
    case MinEdgeCountFact:
        return create_max_vertex_count_fact(ceil((1 + sqrt(1 + 8 * fact->params[0])) / 2));
    case MaxVertexCountFact:
        return create_max_edge_count_fact((fact->params[0]) * (fact->params[0] - 1) / 2);
    default:
        return NULL;
    }
}