/**
 * \file function.c
 * \brief implementation of functions from function.h
 */
#include "function.h"

Function *create_function_by_va_args(char variable, uint8_t coef_num, ...)
{
    GTD_LOG("Creating function by va args, variable = %c, degree = %d", variable, coef_num - 1);
    va_list args;
    Function *f = (Function*)gtd_malloc(sizeof(Function));
    f->variable = variable;
    f->degree = coef_num - 1;
    f->coef = (double *)gtd_malloc((coef_num) * sizeof(double));
    va_start(args, coef_num);
    for (uint8_t i = 0; i < coef_num; i++) {
        f->coef[f->degree - i] = va_arg(args, double);
        GTD_LOG("Assigned f->coef[%d] = %f", i, f->coef[i]);
    }
    va_end(args);
    return f;
}

Function *create_function_by_array(char variable, uint8_t degree, double *coef)
{
    Function *f = (Function*)gtd_malloc(sizeof(Function));
    f->variable = variable;
    f->degree = degree;
    f->coef = (double *)gtd_malloc((degree + 1) * sizeof(double));
    for (uint8_t i = 0; i <= degree; i++) {
        f->coef[i] = coef[i];
    }
    return f;
}

Function *create_constant_integer_function(int32_t c)
{
    return create_function_by_va_args('x', 1, (double)c);
}

void delete_function(Function *f)
{
    if(f == NULL) 
        return;
    if(f->coef != NULL)
        gtd_free(f->coef);
    gtd_free(f);
}

Function *add_functions(const Function *const f1, const Function *const f2)
{
    if(!is_constant(f1) && !is_constant(f2) && f1->variable != f2->variable)
        return NULL;
    uint8_t max_degree = (f1->degree > f2->degree) ? f1->degree : f2->degree;
    double *coefs = (double *)gtd_malloc((max_degree + 1) * sizeof(double));
    for (uint8_t i = 0; i <= max_degree; i++)
    {
        coefs[i] = ((f1->degree >= i) ? f1->coef[i] : 0) + ((f2->degree >= i) ? f2->coef[i] : 0);
    }
    char variable = is_constant(f1) ? f2->variable : f1->variable;
    Function *ret = create_function_by_array(variable, max_degree, coefs);
    gtd_free(coefs);
    return ret;
}

void add_constant(Function *f, const double c)
{
    f->coef[0] += c;
}

Function *subtract_functions(const Function *const f1, const Function *const f2)
{
    if(!is_constant(f1) && !is_constant(f2) && f1->variable != f2->variable)
        return NULL;
    uint8_t max_degree = (f1->degree > f2->degree) ? f1->degree : f2->degree;
    double *coefs = (double *)gtd_malloc((max_degree + 1) * sizeof(double));
    for (uint8_t i = 0; i <= max_degree; i++)
    {
        coefs[i] = ((f1->degree >= i) ? f1->coef[i] : 0) - ((f2->degree >= i) ? f2->coef[i] : 0);
    }
    char variable = is_constant(f1) ? f2->variable : f1->variable;
    Function *ret = create_function_by_array(variable, max_degree, coefs);
    gtd_free(coefs);
    return ret;
}

void subtract_constant(Function *f, const double c)
{
    f->coef[0] -= c;
}

Function *multiply_functions(const Function *const f1, const Function *const f2)
{
    if(!is_constant(f1) && !is_constant(f2) && f1->variable != f2->variable)
        return NULL;
    uint8_t degree = f1->degree + f2->degree;
    double *coef = (double *)gtd_calloc(degree + 1, sizeof(double));
    for (uint8_t i = 0; i <= f1->degree; i++)
    {
        for (uint8_t j = 0; j <= f2->degree; j++)
        {
            coef[i+j] += f1->coef[i] * f2->coef[j];
        }
    }
    char variable = is_constant(f1) ? f2->variable : f1->variable;
    Function *ret = create_function_by_array(variable, degree, coef);
    gtd_free(coef);
    return ret;
}

void multiply_constant(Function *f, const double c)
{
    for(uint8_t i=0; i<=f->degree; i++)
    {
        f->coef[i] *= c;
    }
}

Function *divide_functions(const Function *const f1, const Function *const f2) {
    if(!is_constant(f1) && !is_constant(f2) && f1->variable != f2->variable)
        return NULL;
    if (f2->degree == 0 && f2->coef[0] == 0) {
        return NULL;
    }
    if (f1->degree < f2->degree) {
        return NULL;
    }

    uint8_t result_degree = f1->degree - f2->degree;

    double *coef = (double *)gtd_calloc(result_degree + 1, sizeof(double));
    double *remainder = (double *)gtd_malloc((f1->degree + 1) * sizeof(double));

    memcpy(remainder, f1->coef, (f1->degree + 1) * sizeof(double));

    for (uint8_t i = f1->degree; i >= f2->degree && i != (uint8_t)(-1); i--) {
        if (remainder[i] != 0) {
            double res = remainder[i] / f2->coef[f2->degree];
            coef[i - f2->degree] = res;
            for (int j = f2->degree; j >= 0; j--) {
                remainder[i - (f2->degree - j)] -= res * f2->coef[j];
            }
        }
    }
    bool remainder_zero = true;
    for(uint8_t i=0; i<f1->degree; i++)
    {
        if(remainder[i] != 0)
        {
            remainder_zero = false;
            break;
        }
    }
    char variable = is_constant(f1) ? f2->variable : f1->variable;
    Function *result = remainder_zero ? create_function_by_array(variable, result_degree, coef) : NULL;

    gtd_free(remainder);
    gtd_free(coef);
    return result;
}

int8_t divide_constant(Function *f, const double c)
{
    if(c == 0)
        return OP_NOT_POSSIBLE;
    for(uint8_t i=0; i<=f->degree; i++)
    {
        f->coef[i] /= c;
    }
    return 0;
}

int32_t mod_function(const Function *const f, const int32_t c)
{
    return is_constant(f) ? (int32_t)f->coef[0] % c : OP_NOT_POSSIBLE;
}

Function *sqrt_function(const Function *const f) {
    return is_constant(f) ? create_function_by_va_args(f->variable, 1, sqrt(f->coef[0])) : NULL;
}

int8_t compare_functions(const Function *const f1, const Function *const f2)
{
    if(f1 == NULL)
        return f2 == NULL ? 0 : NULL_ARGUMENT;
    if(f2 == NULL)
        return NULL_ARGUMENT;
    if(f1->variable != f2->variable)
        return OP_NOT_POSSIBLE;
    if(f1->degree != f2->degree)
        return OP_NOT_POSSIBLE;
    for(uint8_t i=f1->degree; i != (uint8_t)(-1); i--)
    {
        if(f1->coef[i] - f2->coef[i] > 1e-9) 
            return 1;
        if(f1->coef[i] - f2->coef[i] < -1e-9)
            return -1;
    }
    return 0;
}

int8_t is_greater_for_parameter(const Function *const f1, const Function *const f2, double x)
{
    GTD_LOG("f1 = %s, f2 = %s, x = %f", get_function_str(f1), get_function_str(f2), x);
    GTD_LOG("degree(f1) = %d, variable(f1) = %c, degree(f2) = %d, variable(f2) = %c", f1->degree, f1->variable, f2->degree, f2->variable);
    GTD_LOG("Coef f1:");
    for(uint8_t i=0; i<=f1->degree; i++)
    {
        GTD_LOG("%f", f1->coef[i]);
    }
    GTD_LOG("Coef f2:");
    for(uint8_t i=0; i<=f2->degree; i++)
    {
        GTD_LOG("%f", f2->coef[i]);
    }
    if(f1->variable != f2->variable || f1->degree > 2 || f2->degree > 2)
        return OP_NOT_POSSIBLE;
    double a,b,c;
    a = (f1->degree >= 2 ? f1->coef[2] : 0) - (f2->degree >= 2 ? f2->coef[2] : 0);
    b = (f1->degree >= 1 ? f1->coef[1] : 0) - (f2->degree >= 1 ? f2->coef[1] : 0);
    c = f1->coef[0] - f2->coef[0];
    double delta = b*b - 4 * a * c;
    double vertex = -b / (2*a);
    GTD_LOG("a = %f, b = %f, c = %f, delta = %f, vertex = %f");
    if(delta <= 0)
    {
        return a > 0 ? 1 : 0;
    }
    else
    {
        double x1 = vertex + sqrt(delta);
        return x >= x1 ? 1 : 0;
    }
}

bool is_constant(const Function *const f)
{
    return f->degree == 0;
}

bool is_equal_constant_function(const Function *const f, double c)
{
    return is_constant(f) && fabs(f->coef[0] - c) < 1e-9;
}

char *get_function_str(const Function *const f)
{
    char *result = (char*)gtd_malloc(1000 * sizeof(char));
    sprintf(result, "(");
    for(uint8_t i=f->degree; i > 0; i--)
    {
        if(f->coef[i] != 0)
        {
            if(i != f->degree || f->coef[i] < 0)
                sprintf(result + strlen(result),f->coef[i] > 0 ? " + " : " - ");
            sprintf(result + strlen(result),"%.1f%c^%d",fabs(f->coef[i]), f->variable, i);
        }
    }
    if(f->coef[0] != 0 && f->degree > 0)
        sprintf(result + strlen(result),f->coef[0] > 0 ? " + %.0f" : " - %.0f", fabs(f->coef[0]));    
    if(f->coef[0] != 0 && f->degree == 0)
        sprintf(result + strlen(result),"%.0f", f->coef[0]);        
    sprintf(result + strlen(result), ")");
    return result;
}

Function *copy_function(const Function *const f) {
    return create_function_by_array(f->variable, f->degree, f->coef);
}
