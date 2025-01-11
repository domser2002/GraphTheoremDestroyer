#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "json_parser.h"
#include "module.h"
#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
    GTD_UNUSED(argc);
    GTD_UNUSED(argv);
    GTD_LOG("GraphTheoremDestroyer initiated. Starting application");
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

    GTD_LOG("Frontend connected");
    size_t pathname_len;    
    char *pathname = create_restrictions_file(&pathname_len);
    send(new_socket, pathname, pathname_len, 0);
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
        free_module_args(module_args);
    }

    close(new_socket);
    GTD_LOG("Frontend disconnected");

    close(server_fd);
    GTD_LOG("Terminating application");
    return EXIT_SUCCESS;
}