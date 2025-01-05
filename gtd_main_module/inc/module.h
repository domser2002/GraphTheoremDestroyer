#ifndef MODULE_H
#define MODULE_H
#define _GNU_SOURCE
#include "fact.h"
#include "fact_tree_main.h"
#include "generative_main.h"
#include "module_args.h"
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#define MODULES_COUNT 2
typedef void* (*module_main_loop)(void *);

/**
 * \brief function to run all modules supported by graph theorem destroyer
 * \param args module args without read_fd and write_fd file descriptors opened
*/
void run_modules(ModuleArgs *args);

#endif