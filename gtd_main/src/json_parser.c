/**
 * \file json_parser.c
 * \brief implementation of functions from json_parser.h
 */
#include "json_parser.h"

ModuleArgs *get_module_args_from_json(const char *pathname)
{
    ModuleArgs *args = (ModuleArgs *)gtd_calloc(1, sizeof(ModuleArgs));
    JSON_Value *root = json_parse_file(pathname);
    JSON_Array *json_fact_array = json_value_get_array(root);
    args->fact_count = (uint32_t)json_array_get_count(json_fact_array);
    args->fact_array = (Fact **)gtd_malloc(args->fact_count * sizeof(Fact*));
    for (size_t i = 0; i < json_array_get_count(json_fact_array); i++) {
        JSON_Object *json_fact = json_array_get_object(json_fact_array, i);
        const char *name = json_object_dotget_string(json_fact, "name");
        FactType type = get_fact_type_by_name(name);
        if (type == UnknownType) {
            GTD_LOG("Error in parsing JSON: Unknown fact type");
            exit(EXIT_FAILURE);
        }
        char **param_names;
        bool *functional;
        uint8_t params_count = get_params(type, &param_names, &functional);
        Function **params = (Function **)gtd_malloc(params_count * sizeof(Function*));
        JSON_Object *int_params = json_object_dotget_object(json_fact, "int_params_values");
        JSON_Object *functional_params = json_object_dotget_object(json_fact, "functions");
        uint8_t loop_start = 0;
        if(type == HasCompletePartiteFact || type == HasNoCompletePartiteFact ||
        type == HasInducedCompletePartiteFact || type == HasNoInducedCompletePartiteFact ||
        type == HasMinorCompletePartiteFact || type == HasNoMinorCompletePartiteFact)
        {
            params[0] = create_constant_integer_function(2);
            loop_start = 1;
        }
        for (uint8_t j = loop_start; j < params_count; j++) {
            const char *param_name = param_names[j - loop_start];
            if(functional[j - loop_start])
            {
                JSON_Object *current_function = json_object_dotget_object(functional_params, param_name);
                const char* var_str = json_object_dotget_string(current_function, "var");
                if(strlen(var_str) != 1)
                {
                    GTD_LOG_DEPLOYMENT("Wrong length of variable name");
                    exit(EXIT_FAILURE);
                }
                char var = var_str[0];
                JSON_Array *coeficients_json = json_object_dotget_array(current_function, "function");
                size_t number_of_coefficients = json_array_get_count(coeficients_json);
                int32_t max_degree = 0;
                for(size_t k=0; k<number_of_coefficients; k++)
                {
                    JSON_Array *coeficient_json = json_array_get_array(coeficients_json, k);
                    int32_t degree = (int32_t)json_array_get_number(coeficient_json, 2);
                    if(degree > max_degree)
                        max_degree = degree;
                }
                double *coeficients = (double*)gtd_calloc(max_degree + 1, sizeof(double));
                for(size_t k=0; k<number_of_coefficients; k++)
                {
                    JSON_Array *coeficient_json = json_array_get_array(coeficients_json, k);
                    int32_t nominator = (int32_t)json_array_get_number(coeficient_json, 0);
                    int32_t denominator = (int32_t)json_array_get_number(coeficient_json, 1);
                    int32_t degree = (int32_t)json_array_get_number(coeficient_json, 2);
                    coeficients[degree] = (double)nominator/denominator;
                }
                params[j] = create_function_by_array(var, max_degree, coeficients);
            }
            if(!functional[j - loop_start])
            {
                int32_t param;
                JSON_Value *param_value = json_object_get_value(int_params, param_name);
                // parson library sometimes interprets number as string
                if (json_value_get_type(param_value) == JSONString) {
                    const char *param_str = json_value_get_string(param_value);
                    param = atoi(param_str);
                } else if (json_value_get_type(param_value) == JSONNumber) {
                    param = (int32_t)json_value_get_number(param_value);
                } else {
                    GTD_LOG("Unexpected type for parameter %s\n", param_name);
                    exit(EXIT_FAILURE);
                }
                params[j] = create_constant_integer_function(param);
            }
        }
        args->fact_array[i] = create_fact(type, params);
    }
    json_value_free(root);
    return args;
}