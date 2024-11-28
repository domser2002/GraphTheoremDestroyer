#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include "log.h"
#define GTD_UNUSED(var) (void)var

void *gtd_malloc(size_t size);
void *gtd_calloc(size_t nmemb, size_t size);
void *gtd_realloc(void *ptr, size_t size);
void gtd_free(void *ptr);
#endif

void printProgressBar(int progress, int total);