#include "physical_graph.h"
#include "generative_restriction.h"

struct GenerativeRestriction
{
    int (*restriction)(Graph* graph, int *params);
    int num_params;
    int *params;
};

GenerativeRestriction *create_restriction(int (*restriction)(Graph* graph, int *params), int num_params, int *params)
{
    GenerativeRestriction *genRestriction = gtd_malloc(sizeof(GenerativeRestriction));
    genRestriction->restriction = restriction;
    genRestriction->num_params = num_params;
    genRestriction->params = params;
    return genRestriction;
}

int destroy_restriction(GenerativeRestriction *genRestriction)
{
    if(genRestriction != NULL)
    {
        gtd_free(genRestriction->params);
        gtd_free(genRestriction);
    }
    return 1;
}

int check_restriction(Graph *graph, GenerativeRestriction *restriction)
{
    int res = restriction->restriction(graph, restriction->params);
    return res;
}

GenerativeRestriction *create_max_vertices_restriction(int max_vertices)
{
    int *params = gtd_malloc(sizeof(int));
    params[0] = max_vertices;
    GenerativeRestriction *restrinction = create_restriction(max_vertices_restriction_condition, 1, params);
    return restrinction;
}

int max_vertices_restriction_condition(Graph* graph, int *params)
{
    int vertices_num = get_graph_num_vertices(graph);
    int max_vertices = params[0];

    if(vertices_num == max_vertices)
    {
        set_graph_maximal(graph);
    }

    return vertices_num <= max_vertices ? 1 : 0;
}
