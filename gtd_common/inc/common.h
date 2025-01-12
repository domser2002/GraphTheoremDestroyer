/**
 * \file common.h
 * \brief header which contains wrappers for memory allocation functions and common macros
 * \note memory allocation function should not be used anywhere in GTD directly, use only wrappers from this file
 */
#ifndef COMMON_H
#define COMMON_H
#define MAX_PATHNAME_LEN 1024
#include <stdlib.h>
#include "log.h"
#include <stdint.h>
#define GTD_UNUSED(var) (void)var

/**
 * \brief wrapper for malloc
 */
void *gtd_malloc(size_t size);

/**
 * \brief wrapper for calloc
 */
void *gtd_calloc(size_t nmemb, size_t size);

/**
 * \brief wrapper for realloc
 */
void *gtd_realloc(void *ptr, size_t size);

/**
 * \brief wrapper for free
 */
void gtd_free(void *ptr);

#endif