#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>

#define GTD_LOG(...) write_log_to_file("log.txt",__VA_ARGS__)
extern bool test_mode;

void write_log_to_stdout(const char *format, ...);
void write_log_to_file(const char *filepath, const char *format, ...);

#endif
