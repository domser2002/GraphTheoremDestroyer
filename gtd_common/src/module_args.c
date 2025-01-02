#include "module_args.h"

ModuleArgs *deep_copy_module_args(ModuleArgs *args)
{
    ModuleArgs *new_args = (ModuleArgs*)gtd_malloc(sizeof(ModuleArgs));
    new_args->fact_array = deep_copy_fact_array(args->fact_array, args->fact_count);
    new_args->fact_count = args->fact_count;
    new_args->max_vertices = args->max_vertices;
    new_args->start_graph = copy_graph(args->start_graph);
    if (args->min_vertices) {
        new_args->min_vertices = (uint32_t *)gtd_malloc(sizeof(uint32_t));
        *new_args->min_vertices = *args->min_vertices;
    } else {
        new_args->min_vertices = NULL;
    }
    new_args->read_fd = args->read_fd;
    new_args->write_fd = args->write_fd;
    return new_args;
}

void free_module_args(ModuleArgs *args)
{
    gtd_free(args->fact_array);
    gtd_free(args->min_vertices);
    destroy_graph(args->start_graph);
    gtd_free(args);
}