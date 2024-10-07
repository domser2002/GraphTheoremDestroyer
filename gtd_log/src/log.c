#include "log.h"

static void format_and_write(const char *format, FILE *output, va_list args)
{
    vfprintf(output, format, args); 
    fprintf(output, "\n"); 
}

void write_log_to_stdout(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    format_and_write(format, stdout, args);
    va_end(args);
}

void write_log_to_file(const char *filepath, const char *format, ...)
{
    FILE *output = fopen(filepath, "a");
    if (output == NULL) return; 
    va_list args;
    va_start(args, format);
    format_and_write(format, output, args);
    va_end(args);
    fclose(output);
}
