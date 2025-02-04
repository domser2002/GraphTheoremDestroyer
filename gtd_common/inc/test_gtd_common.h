/**
 * \file test_gtd_common.h
 * \brief header to run all tests for gtd_common
 */
#ifndef TEST_GTD_COMMON_H
#define TEST_GTD_COMMON_H
#define _GNU_SOURCE
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "fact.h"
#include "module_args.h"
void gtd_common_unit_tests(void);
void gtd_common_integration_tests(void);
#endif