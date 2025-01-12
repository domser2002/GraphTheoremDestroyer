#ifndef GENERATIVE_MAIN_H
#define GENERATIVE_MAIN_H
#include <unistd.h>
#include "fact.h"
#include "messages.h"
#include "common.h"
#include "log.h"
#include "module_args.h"
/**
 * \brief main loop for generative module, which executes it with starting fact array 
 * and handles messages on file descriptors
 * \param argument pointer to module args, below list of the params used by this module 
 * \param fact_array array of facts received from frontend, to be converted to restrictions
 * \param fact_count number of facts in array
 * \param start_graph optional graph from which generating is started
 * \param min_vertices optional min number of vertices for generated graph, default is 1
 * \param max_vertices max number of vertices for generated graph
 * \param write_fd file descriptor to the write end of pipe which generative module will
 *  use for writting to main module
 * \param read_fd file descriptor to the read end of pipe which generative module will
 * use for reading from main module
 * \returns NULL
*/
void *generative_main_loop(void *argument);
#endif