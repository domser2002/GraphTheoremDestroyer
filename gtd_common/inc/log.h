#ifndef LOG_H
#define LOG_H
#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_THREAD_NAME_LEN 50
#define GTD_LOG(...) write_log_to_file("log.txt",__VA_ARGS__)
extern bool test_mode;

void write_log_to_stdout(const char *format, ...);
void write_log_to_file(const char *filepath, const char *format, ...);
void cleanup(void);

#endif
