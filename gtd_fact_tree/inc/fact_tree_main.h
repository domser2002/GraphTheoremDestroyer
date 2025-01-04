#ifndef FACT_TREE_MAIN_H
#define FACT_TREE_MAIN_H
#include "fact.h"
#include "messages.h"
#include "common.h"
#include "log.h"
#include "module_args.h"
#include <unistd.h>

/**
 * \brief main loop for fact tree module, which executes it with starting fact array 
 * and handles messages on file descriptors
 * \param argument pointer to module args, below list of the params used by this module
 * \param fact_array array of facts received from frontend
 * \param fact_count number of facts in array
 * \param write_fd file descriptor to the write end of pipe which fact tree module will
 *  use for writting to main module
 * \param read_fd file descriptor to the read end of pipe which fact tree module will
 * use for reading from main module
 * \returns NULL
*/
void *fact_tree_main_loop(void *argument);
#endif