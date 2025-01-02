#define _GNU_SOURCE
#include <stdio.h>
#include "json_parser.h"
#include "module.h"

int main(void)
{
    GTD_LOG("GraphTheoremDestroyer initiated. Starting application");
    ModuleArgs *module_args = get_module_args_from_json(NULL);
    GTD_LOG("Got module args from json");
    run_modules(module_args);
    GTD_LOG("Terminating application");
    return EXIT_SUCCESS;
}