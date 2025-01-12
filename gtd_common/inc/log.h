/**
 * \file log.h
 * \brief header including functions and macros allowing to generate logs
 * \note by changing this file it is possible to switch between logs printed to file and logs printed to stdout
 */
#ifndef LOG_H
#define LOG_H
#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_THREAD_NAME_LEN 50

/**
 * \brief macro to print log message
 */
#define GTD_LOG(...) write_log_to_file("log.txt",__VA_ARGS__)
extern bool test_mode;

/**
 * \brief function to write log message to stdout
 * \warning should not be called directly, use macro instead
 */
void write_log_to_stdout(const char *format, ...);

/**
 * \brief function to write log message to file
 * \warning should not be called directly, use macro instead
 */
void write_log_to_file(const char *filepath, const char *format, ...);

/**
 * \brief function to cleanup memory allocated by logs
 */
void cleanup(void);

#endif
