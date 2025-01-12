/**
 * \file module_args.h
 * \brief header containing struct to be passed as an argument to modules main loop and functions to deep copy and delete it
 */
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

/**
 * \brief function to create a deep copy of ModuleArgs
 */
ModuleArgs *deep_copy_module_args(ModuleArgs *args);

/**
 * \brief function to delete (free) ModuleArgs
 */
void free_module_args(ModuleArgs *args);
#endif