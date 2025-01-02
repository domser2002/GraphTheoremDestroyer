#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#define OP_NOT_POSSIBLE INT8_MAX
#define NULL_ARGUMENT INT8_MAX-1
#include "common.h"
typedef struct Function {
    int32_t c;
} Function;

/**
 * \brief constructor for Function class
 * \param c polynomial free term 
*/
Function *create_function(int32_t c);

/**
 * \brief destructor for Function class
*/
void delete_function(Function *f);

/**
 * \brief function to add 2 Function objects
 * \returns function which is a result of addition
*/
Function *add_functions(const Function *const f1, const Function *const f2);

/**
 * \brief function to add a constant to Function object
 * \param f function to which we add
*/
void add_constant(Function *f, const int32_t c);

/**
 * \brief function to subtract 2 Function objects
 * \returns function which is a result of subtraction
*/
Function *subtract_functions(const Function *const f1, const Function *const f2);

/**
 * \brief function to subtract a constant from Function object
 * \param f function from which we subtract
*/
void subtract_constant(Function *f, const int32_t c);

/**
 * \brief function to multiply 2 Function objects
 * \returns function which is a result of multiplication
*/
Function *multiply_functions(const Function *const f1, const Function *const f2);

/**
 * \brief function to multiply a Function object by a constant
 * \param f function which we multiply
*/
void multiply_constant(Function *f, const int32_t c);

/**
 * \brief function to divide 2 Function objects
 * \returns function which is a result of division, if the division is possible, NULL otherwise
*/
Function *divide_functions(const Function *const f1, const Function *const f2);

/**
 * \brief function to divide a Function object by a constant
 * \param f function which we divide if it is possible
 * \returns OP_NOT_POSSIBLE if the division is not possible, 0 otherwise
*/
int8_t divide_constant(Function *f, const int32_t c);

/**
 * \brief function to calculate f % c, only for constant function
 * \returns f % c if f is constant, OP_NOT_POSSIBLE otherwise
*/
int32_t mod_function(const Function *const f, const int32_t c);

/**
 * \brief function to calculate sqrt of a function
 * \returns function which is a result of sqrt
*/
Function *sqrt_function(const Function *const f);

/**
 * \brief function to compare 2 Function objects
 * \returns 1 if f1 > f2, 0 if f1 == f2, -1 if f1 < f2, OP_NOT_POSSIBLE if it depends on argument, NULL_ARGUMENT if exactly one function is NULL
 * \note if both functions are NULL, 0 is returned
*/
int8_t compare_functions(const Function *const f1, const Function *const f2);

/**
 * \brief function to check if Function object is a constant function
*/
bool is_constant(const Function *const f);

/**
 * \brief function to check if Function object is equal to a constant
*/
bool is_equal_constant_function(const Function *const f, int32_t c);

/**
 * \brief get a string to print function object
*/
char *get_function_str(const Function *const f);

/**
 * \brief create a deep copy of Function object
*/
Function *copy_function(const Function *const f);

/**
 * \brief function to convert constant function array to int32_t array
 * \param func_array array of functions
 * \param array_size number of functions
 * \returns array of int32_t or NULL if functions are not constant 
*/
int32_t *flatten(Function **func_array, uint32_t array_size);
#endif