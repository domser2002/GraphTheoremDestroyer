#ifndef MODULE_ARGS_H
#define MODULE_ARGS_H
#include "fact.h"
#include "physical_graph.h"
#include "common.h"

typedef struct ModuleArgs {
    // required theorem parameters
    Fact **fact_array;
    uint32_t fact_count;
    // out file
    char out_file_path[MAX_PATHNAME_LEN];
    // communication parameters
    int write_fd; // must be int, defined by POSIX
    int read_fd;
} ModuleArgs;

void set_out_file_path(ModuleArgs *args, size_t *pathname_len);
ModuleArgs *deep_copy_module_args(ModuleArgs *args);
void free_module_args(ModuleArgs *args);
#endif