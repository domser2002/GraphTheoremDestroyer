#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#define OP_NOT_POSSIBLE 0xFFF
#include "common.h"
typedef struct Function {
    int32_t c;
} Function;

/**
 * \brief constructor for Function class
 * \param c polynomial free term 
*/
Function *create(int32_t c);

/**
 * \brief destructor for Function class
*/
void delete_function(Function *f);

/**
 * \brief function to add 2 Function objects
 * \param result function which is a result of addition
*/
void add(const Function *const f1, const Function *const f2, Function *result);

/**
 * \brief function to add a constant to Function object
 * \param result function which is a result of addition
*/
void add_constant(const Function *const f, const int32_t c, Function *result);

/**
 * \brief function to subtract 2 Function objects
 * \param result function which is a result of subtraction
*/
void subtract(const Function *const f1, const Function *const f2, Function *result);

/**
 * \brief function to subtract a constant from Function object
 * \param result function which is a result of subtraction
*/
void subtract_constant(const Function *const f1, const int32_t c, Function *result);

/**
 * \brief function to multiply 2 Function objects
 * \param result function which is a result of multiplication
*/
void multiply(const Function *const f1, const Function *const f2, Function *result);

/**
 * \brief function to multiply a Function object by a constant
 * \param result function which is a result of multiplication
*/
void multiply_constant(const Function *const f1, const int32_t c, Function *result);

/**
 * \brief function to divide 2 Function objects
 * \param result function which is a result of division, if the division is possible, NULL otherwise
*/
void divide(const Function *const f1, const Function *const f2, Function *result);

/**
 * \brief function to divide a Function object by a constant
 * \param result function which is a result of division, if the division is possible, NULL otherwise
*/
void divide_constant(const Function *const f1, const int32_t c, Function *result);

/**
 * \brief function to calculate f % c, only for constant function
 * \returns f % c if f is constant. OP_NOT_POSSIBLE otherwise
*/
int8_t mod_function(const Function *const f, const int8_t c);

void sqrt_function(const Function *const f, Function *result);

/**
 * \brief function to compare 2 Function objects
 * \returns 1 if f1 > f2, 0 if f1 == f2, -1 if f1 < f2, OP_NOT_POSSIBLE if it depends on argument
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
void copy_function(const Function *src, Function **dest);
#endif