#include "common.h"

void *gtd_malloc(size_t size)
{
    void *ptr = malloc(size);
    return ptr;
}

void *gtd_calloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb,size);
    return ptr;
}

void *gtd_realloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    return ptr;
}

void gtd_free(void *ptr)
{
    free(ptr);
    ptr = NULL; // to prevent dangling pointers
    return;
}