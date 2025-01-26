/**
 * \file main.c
 * \brief main file containing entry point of a Graph Theorem Destroyer (GTD) application
 * \details This file includes main function which is an entry point to GTD. This function handles
 * all necessary initialization at the beginning and cleaning/freeing memory at the end. It is
 * also responsible for gathering results of shared work of all modules and communicating with
 * frontend application.  
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "json_parser.h"
#include "module.h"
#define PORT 8711
#define BUFFER_SIZE 1024

/**
 * \brief function to create a json file with a list of supported restriction/fact types with parameters
 * \param pathname_len length of file name
 * \returns path to the created file
 */
char *create_restrictions_file(size_t *pathname_len)
{
    char *pathname = (char *)gtd_malloc(MAX_PATHNAME_LEN * sizeof(char));
    char time_str[20];
    char cwd[MAX_PATHNAME_LEN];
    time_t rawtime;
    struct tm *timeinfo;

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(time_str, sizeof(time_str), "%Y%m%d%H%M%S", timeinfo);
        sprintf(pathname, "%s/metadata/restrictions_%s.json", cwd, time_str);
        *pathname_len = strlen(pathname);
    }
    else
    {
        GTD_LOG("getcwd() error");
        exit(EXIT_FAILURE);
    }

    JSON_Value *root_value = json_value_init_array();
    JSON_Array *root_array = json_value_get_array(root_value);
    for(uint32_t i=0; i < FACT_TYPE_NUM; i++)
    {
        JSON_Value *item_value;
        JSON_Object *item_object;
        JSON_Value *int_params_value;
        JSON_Array *int_params_array;
        JSON_Value *functions_value;
        JSON_Array *functions_array;
        bool any_int = false;
        bool any_functional = false;
        item_value = json_value_init_object();
        item_object = json_value_get_object(item_value);
        json_object_set_number(item_object, "id", i);
        json_object_set_string(item_object, "name", get_fact_type_name(i));
        char **param_names;
        bool *functional;
        uint8_t param_count = get_params(i, &param_names, &functional);
        int_params_value = json_value_init_array();
        int_params_array = json_value_get_array(int_params_value);
        functions_value = json_value_init_array();
        functions_array = json_value_get_array(functions_value);
        for(uint8_t j=0; j<param_count; j++)
        {
            if(functional[j])
            {
                any_functional = true;
                json_array_append_string(functions_array, param_names[j]);
            }
            else
            {
                any_int = true;
                json_array_append_string(int_params_array, param_names[j]);
            }
        }
        if(any_int)
            json_object_set_value(item_object, "int_params", int_params_value);
        if(any_functional)
            json_object_set_value(item_object, "functions", functions_value);
        json_array_append_value(root_array, item_value);
        gtd_free(param_names);
        gtd_free(functional);
    }
    json_serialize_to_file_pretty(root_value, pathname);

    json_value_free(root_value);
    return pathname;
}

/**
 * \brief function to set a path under which proof will be written
 */
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

/**
 * \brief entry point for GTD
 */
int main(int argc, char **argv)
{
    log_level = DEPLOYMENT_LOGS;
    if(argc == 2 && strcmp(argv[1], "debug") == 0)
    {
        log_level = DEBUG_LOGS;
    }
    else if(argc == 2 && strcmp(argv[1], "no") == 0)
    {
        log_level = NO_LOGS;
    }
    GTD_LOG_DEPLOYMENT("GraphTheoremDestroyer initiated. Starting application");
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        GTD_LOG("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        GTD_LOG("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        GTD_LOG("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        GTD_LOG("listen failed");
        exit(EXIT_FAILURE);
    }

    GTD_LOG("Waiting for frontend to connect");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        GTD_LOG("accept failed");
        exit(EXIT_FAILURE);
    }

    GTD_LOG_DEPLOYMENT("Frontend connected");
    size_t pathname_len;    
    char *pathname = create_restrictions_file(&pathname_len);
    send(new_socket, pathname, pathname_len, 0);
    gtd_free(pathname);
    GTD_LOG("Sent restrictions to frontend");
    int read_size;
    while ((read_size = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        GTD_LOG("Received JSON filename: %s", buffer);
        ModuleArgs *module_args = get_module_args_from_json(buffer);
        GTD_LOG("Got module args from json");
        size_t out_file_pathname_len;
        set_out_file_path(module_args, &out_file_pathname_len);
        run_modules(module_args);
        send(new_socket, module_args->out_file_path, out_file_pathname_len, 0);
        GTD_LOG_DEPLOYMENT("Sent proof details to frontend: %s", module_args->out_file_path);
        free_module_args(module_args);
    }

    close(new_socket);
    GTD_LOG_DEPLOYMENT("Frontend disconnected");

    close(server_fd);
    GTD_LOG_DEPLOYMENT("Terminating application");
    cleanup();
    return EXIT_SUCCESS;
}