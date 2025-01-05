#define _GNU_SOURCE
#include <stdio.h>
#include "json_parser.h"
#include "module.h"

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s [json_pathname]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    GTD_LOG("GraphTheoremDestroyer initiated. Starting application");
    ModuleArgs *module_args = get_module_args_from_json(argv[1]);
    GTD_LOG("Got module args from json");
    run_modules(module_args);
    GTD_LOG("Terminating application");
    return EXIT_SUCCESS;
}