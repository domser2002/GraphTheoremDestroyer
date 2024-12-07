#include "function.h"

Function *create(int32_t c)
{
    Function *f = (Function*)gtd_malloc(sizeof(Function));
    f->c = c;
    return f;
}

void delete_function(Function *f)
{
    gtd_free(f);
}

void add(const Function *const f1, const Function *const f2, Function *result)
{
    result->c = f1->c + f2->c;
}

void add_constant(const Function *const f, const int32_t c, Function *result)
{
    result->c = f->c + c;
}

void subtract(const Function *const f1, const Function *const f2, Function *result)
{
    result->c = f1->c - f2->c;
}

void subtract_constant(const Function *const f, const int32_t c, Function *result)
{
    if(result == NULL)
    {
        result = (Function*)gtd_malloc(sizeof(Function));
    }
    result->c = f->c - c;
}

void multiply(const Function *const f1, const Function *const f2, Function *result)
{
    result->c = f1->c * f2->c;
}

void multiply_constant(const Function *const f, const int32_t c, Function *result)
{
    result->c = f->c * c;
}

void divide(const Function *const f1, const Function *const f2, Function *result)
{
    result->c = f1->c / f2->c;
}

void divide_constant(const Function *const f, const int32_t c, Function *result)
{
    result->c = f->c / c;
}

int8_t mod_function(const Function *const f, const int8_t c)
{
    return f->c % c;
}

void sqrt_function(const Function *const f, Function *result) {
    result->c = (int32_t)sqrt(f->c);
}

int8_t compare_functions(const Function *const f1, const Function *const f2)
{
    if(f1->c > f2->c) 
        return 1;
    if(f1->c == f2->c)
        return 0;
    return -1;
}

bool is_constant(const Function *const f)
{
    GTD_UNUSED(f);
    return true;
}

bool is_equal_constant_function(const Function *const f, int32_t c)
{
    return is_constant(f) && f->c == c;
}

char *get_function_str(const Function *const f)
{
    char *result = (char*)gtd_malloc(12*sizeof(char));
    if(result == NULL)
    {
        GTD_LOG("out of memory");
        return NULL;
    }
    sprintf(result,"%d",f->c);
    return result;
}

void copy_function(const Function *src, Function **dest) {
    *dest = create(src->c);
}
