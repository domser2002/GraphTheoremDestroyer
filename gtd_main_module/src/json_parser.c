#include "json_parser.h"

ModuleArgs *get_module_args_from_json(const char *pathname)
{
    GTD_UNUSED(pathname);
    ModuleArgs *args = (ModuleArgs *)gtd_calloc(1, sizeof(ModuleArgs));
    // // erdos-gyarvas
    // args->fact_count = 5;
    // args->fact_array = (Fact **)gtd_malloc(args->fact_count * sizeof(Fact*));
    // Function *params0[1];
    // params0[0] = create_function(4);
    // Function *params1[1];
    // params1[0] = create_function(8);
    // Function *params2[1];
    // params2[0] = create_function(8);
    // Function *params3[1];
    // params3[0] = create_function(3);
    // Function *params4[1];
    // params4[0] = create_function(3);
    // args->fact_array[0] = create_fact(HasNoCycleFact, params0);
    // args->fact_array[1] = create_fact(HasNoCycleFact, params1);
    // args->fact_array[2] = create_fact(HasNoInducedPathFact, params2);
    // args->fact_array[3] = create_fact(MinVertexDegreeFact, params3);
    // args->fact_array[4] = create_fact(HasNoUnknownEdgesFact, params4);
    // args->max_vertices = 15;
    // args->min_vertices = (uint32_t *)gtd_malloc(sizeof(uint32_t));
    // *args->min_vertices = 7;
    // args->start_graph = create_graph(args->max_vertices, *args->min_vertices);
    // for(uint32_t i = 0; i < *args->min_vertices; ++i)
    // {
    //     for(uint32_t j = 0; j < *args->min_vertices; ++j)
    //     {
    //         if(i == j)
    //         {
    //             continue;
    //         }
    //         if((i+1) % 7 == j || (i-1) % 7 == j)
    //         {
    //             set_edge_connected(args->start_graph, i, j);
    //         }
    //         else
    //         {
    //             set_edge_not_connected(args->start_graph, i, j);
    //         }
    //     }
    // }
    // // fact tree
    // args->fact_count = 5;
    // args->fact_array = (Fact **)gtd_malloc(args->fact_count * sizeof(Fact*));
    // Function *params_1[3];
    // params_1[0] = create_function(2);
    // params_1[1] = create_function(1);
    // params_1[2] = create_function(4);
    // Function *params_2[1];
    // params_2[0] = create_function(4);
    // Function *params_3[1];
    // params_3[0] = create_function(8);
    // args->fact_array[0] = create_fact(HasNoCyclesFact,NULL);
    // args->fact_array[1] = create_fact(HasNoInducedCompletePartiteFact,params_1);
    // args->fact_array[2] = create_fact(HasNoInducedPathFact,params_2);
    // args->fact_array[3] = create_fact(MinVertexCountFact,params_3);
    // args->fact_array[4] = create_fact(IsConnectedFact,NULL);
    // both
    args->fact_count = 10;
    args->fact_array = (Fact **)gtd_malloc(args->fact_count * sizeof(Fact*));
    Function *params0[1];
    params0[0] = create_function(4);
    Function *params1[1];
    params1[0] = create_function(8);
    Function *params2[1];
    params2[0] = create_function(8);
    Function *params3[1];
    params3[0] = create_function(3);
    Function *params4[1];
    params4[0] = create_function(3);
    Function *params_6[3];
    params_6[0] = create_function(2);
    params_6[1] = create_function(1);
    params_6[2] = create_function(4);
    Function *params_7[1];
    params_7[0] = create_function(4);
    Function *params_8[1];
    params_8[0] = create_function(8);
    args->fact_array[0] = create_fact(HasNoCycleFact, params0);
    args->fact_array[1] = create_fact(HasNoCycleFact, params1);
    args->fact_array[2] = create_fact(HasNoInducedPathFact, params2);
    args->fact_array[3] = create_fact(MinVertexDegreeFact, params3);
    args->fact_array[4] = create_fact(HasNoUnknownEdgesFact, params4);
    args->fact_array[5] = create_fact(HasNoCyclesFact,NULL);
    args->fact_array[6] = create_fact(HasNoInducedCompletePartiteFact,params_6);
    args->fact_array[7] = create_fact(HasNoInducedPathFact,params_7);
    args->fact_array[8] = create_fact(MinVertexCountFact,params_8);
    args->fact_array[9] = create_fact(IsConnectedFact,NULL);
    args->max_vertices = 15;
    args->min_vertices = (uint32_t *)gtd_malloc(sizeof(uint32_t));
    *args->min_vertices = 7;
    args->start_graph = create_graph(args->max_vertices, *args->min_vertices);
    for(uint32_t i = 0; i < *args->min_vertices; ++i)
    {
        for(uint32_t j = 0; j < *args->min_vertices; ++j)
        {
            if(i == j)
            {
                continue;
            }
            if((i+1) % 7 == j || (i-1) % 7 == j)
            {
                set_edge_connected(args->start_graph, i, j);
            }
            else
            {
                set_edge_not_connected(args->start_graph, i, j);
            }
        }
    }
    return args;
}