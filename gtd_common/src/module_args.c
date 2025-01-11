#include "module_args.h"

void set_out_file_path(ModuleArgs *args, size_t *pathname_len)
{
    char time_str[20];
    char cwd[MAX_PATHNAME_LEN];
    time_t rawtime;
    struct tm *timeinfo;

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(time_str, sizeof(time_str), "%Y%m%d%H%M%S", timeinfo);
        snprintf(args->out_file_path, MAX_PATHNAME_LEN, "%s/metadata/proof_details_%s.txt", cwd, time_str);
        *pathname_len = strlen(args->out_file_path);
    }
    else
    {
        GTD_LOG("getcwd() error");
        exit(EXIT_FAILURE);
    }
}

ModuleArgs *deep_copy_module_args(ModuleArgs *args)
{
    ModuleArgs *new_args = (ModuleArgs*)gtd_malloc(sizeof(ModuleArgs));
    new_args->fact_array = deep_copy_fact_array(args->fact_array, args->fact_count);
    new_args->fact_count = args->fact_count;
    new_args->read_fd = args->read_fd;
    new_args->write_fd = args->write_fd;
    strcpy(new_args->out_file_path, args->out_file_path);
    return new_args;
}

void free_module_args(ModuleArgs *args)
{
    for(uint32_t i=0; i<args->fact_count; i++)
    {
        delete_fact(args->fact_array[i]);
    }
    gtd_free(args->fact_array);
    gtd_free(args);
}