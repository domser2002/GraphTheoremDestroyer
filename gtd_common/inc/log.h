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

typedef enum LogLevel {
    DEBUG_LOGS,
    DEPLOYMENT_LOGS,
    NO_LOGS
} LogLevel;

/**
 * \brief macro to print log message
 */
#define GTD_LOG(...) write_debug_log_to_file("log.txt",__VA_ARGS__)
#define GTD_LOG_DEPLOYMENT(...) write_deployment_log_to_file("log.txt",__VA_ARGS__)
extern LogLevel log_level;

/**
 * \brief function to write log message to stdout, if log_level is set to DEBUG_LOGS
 * \warning should not be called directly, use macro instead
 */
void write_debug_log_to_stdout(const char *format, ...);

/**
 * \brief function to write log message to stdout, if log_level is set to DEBUG_LOGS or DEPLOYMENT_LOGS
 * \warning should not be called directly, use macro instead
 */
void write_deployment_log_to_stdout(const char *format, ...);

/**
 * \brief function to write log message to file, if log_level is set to DEBUG_LOGS
 * \warning should not be called directly, use macro instead
 */
void write_debug_log_to_file(const char *filepath, const char *format, ...);

/**
 * \brief function to write log message to file, if log_level is set to DEBUG_LOGS or DEPLOYMENT_LOGS
 * \warning should not be called directly, use macro instead
 */
void write_deployment_log_to_file(const char *filepath, const char *format, ...);

/**
 * \brief function to cleanup memory allocated by logs
 */
void cleanup(void);

#endif
