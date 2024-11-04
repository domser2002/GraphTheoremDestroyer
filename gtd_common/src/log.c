#include "log.h"
bool first_log = true;
bool test_mode = false;

static void format_and_write(const char *format, FILE *output, va_list args)
{
    char time_str[80];
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
    // TODO: will need a mutex after introducing multithreading
    // CRITICAL SECTION START
    fprintf(output, "[%s] ", time_str);
    vfprintf(output, format, args);
    fprintf(output, "\n");
    // CRITICAL SECTION END
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
