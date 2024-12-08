#include "function.h"

Function *create_function(int32_t c)
{
    Function *f = (Function*)gtd_malloc(sizeof(Function));
    f->c = c;
    return f;
}

void delete_function(Function *f)
{
    if(f == NULL) 
        return;
    gtd_free(f);
}

Function *add_functions(const Function *const f1, const Function *const f2)
{
    return create_function(f1->c + f2->c);
}

void add_constant(Function *f, const int32_t c)
{
    f->c += c;
}

Function *subtract_functions(const Function *const f1, const Function *const f2)
{
    return create_function(f1->c - f2->c);
}

void subtract_constant(Function *f, const int32_t c)
{
    f->c -= c;
}

Function *multiply_functions(const Function *const f1, const Function *const f2)
{
    return create_function(f1->c * f2->c);
}

void multiply_constant(Function *f, const int32_t c)
{
    f->c *= c;
}

Function *divide_functions(const Function *const f1, const Function *const f2)
{
    if(is_equal_constant_function(f2, 0))
        return NULL;
    return create_function(f1->c / f2->c);
}

int8_t divide_constant(Function *f, const int32_t c)
{
    if(c == 0)
        return OP_NOT_POSSIBLE;
    f->c /= c;
    return 0;
}

int32_t mod_function(const Function *const f, const int32_t c)
{
    return f->c % c;
}

Function *sqrt_function(const Function *const f) {
    return create_function((int32_t)sqrt(f->c));
}

int8_t compare_functions(const Function *const f1, const Function *const f2)
{
    if(f1 == NULL)
        return f2 == NULL ? 0 : NULL_ARGUMENT;
    if(f2 == NULL)
        return NULL_ARGUMENT;

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

Function *copy_function(const Function *const f) {
    return create_function(f->c);
}
