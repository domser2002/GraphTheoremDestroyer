#ifndef MODULE_ARGS_H
#define MODULE_ARGS_H
#include "fact.h"
#include "physical_graph.h"

typedef struct ModuleArgs {
    // required theorem parameters
    Fact **fact_array;
    uint32_t fact_count;
    // communication parameters
    int write_fd; // must be int, defined by POSIX
    int read_fd;
} ModuleArgs;

ModuleArgs *deep_copy_module_args(ModuleArgs *args);
void free_module_args(ModuleArgs *args);
#endif