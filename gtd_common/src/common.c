#include "common.h"

void *gtd_malloc(size_t size)
{
    void *ptr = malloc(size);
    GTD_LOG("malloc %p",ptr);
    return ptr;
}

void *gtd_calloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb,size);
    GTD_LOG("calloc %p",ptr);
    return ptr;
}

void *gtd_realloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    GTD_LOG("realloc %p",ptr);
    return ptr;
}

void gtd_free(void *ptr)
{
    GTD_LOG("free %p",ptr);
    return free(ptr);
}