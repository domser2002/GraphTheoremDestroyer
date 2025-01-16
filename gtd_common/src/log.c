/**
 * \file log.c
 * \brief implementation of functions from log.h
 */
#include "log.h"
bool first_log = true;
bool test_mode = false;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

static void format_and_write(const char *format, FILE *output, va_list args)
{
    char time_str[80];
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
    char thread_name[MAX_THREAD_NAME_LEN];
    pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
    pthread_mutex_lock(&log_mutex);
    fprintf(output, "[%s] (%s) ", time_str, thread_name);
    vfprintf(output, format, args);
    fprintf(output, "\n");
    pthread_mutex_unlock(&log_mutex);
}

void write_log_to_stdout(const char *format, ...)
{
    if (test_mode)
        return;
    va_list args;
    va_start(args, format);
    format_and_write(format, stdout, args);
    va_end(args);
}

void write_log_to_file(const char *filepath, const char *format, ...)
{
    if (test_mode)
        return;
    FILE *output = fopen(filepath, first_log ? "w" : "a");
    if (output == NULL)
        return;
    first_log = false;
    va_list args;
    va_start(args, format);
    format_and_write(format, output, args);
    va_end(args);
    fclose(output);
}

void cleanup(void)
{
    pthread_mutex_destroy(&log_mutex);
}
