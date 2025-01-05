#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include "common.h"
#include "module_args.h"
#include "parson.h"

/**
 * \brief function to parse json received from frontend into ModuleArgs struct
 * \param pathname path to a json file
 * \returns pointer to ModuleArgs without fds opened
 * \note it is a mock, it does not parse json yet
*/
ModuleArgs *get_module_args_from_json(const char *pathname);
#endif