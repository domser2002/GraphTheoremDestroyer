from sympy import symbols

class RestrictionSchema:

    def __init__(self, id, name, int_params: list[str], functions: list[str]):
        self.id = id
        self.name = name
        self.int_params = int_params
        self.functions = functions

    def __repr__(self):
        return f'RestrictionSchema(id: {self.id}, name: {self.name}, int_params: {self.int_params})\n'

class RestrictionParameters:
    def __init__(self, 
                 restriction_id: int,
                 restriction_name: str,
                 int_params_values: dict,
                 functions: dict):
        
        self.int_params_values = int_params_values
        self.restriction_id = restriction_id
        self.restriction_name = restriction_name
        self.functions = functions
    
class Restriction:

    def __init__(self, params: RestrictionParameters):
        id = params.restriction_id
        name = params.restriction_name
        int_params_values = params.int_params_values
        functions = params.functions

        self.id = id
        self.name = name
        self.int_params_values = int_params_values
        self.functions = functions
    
    def to_dictionary(self):
        result = {}
        result['id'] = self.id
        result['name'] = self.name
        if self.int_params_values is not None and len(self.int_params_values) > 0:
            result['int_params_values'] = {}
            for key in self.int_params_values:
                val = self.int_params_values[key]
                result['int_params_values'][key] = val
        if self.functions is not None and len(self.functions) > 0:
            result['functions'] = {}
            for key in self.functions:
                val = self.functions[key]
                val_var = str(val['var'])
                val_poly = val['function']
                result['functions'][key] = {'var': val_var, 'function': val_poly}
        return result
    
    @staticmethod
    def from_dictionary(dictionary: dict):
        id = dictionary['id']
        name = dictionary['name']
        int_params_values = {}
        functions = {}
        if 'int_params_values' in dictionary:
            int_params_values = dictionary['int_params_values']
        if 'functions' in dictionary:
            functions = dictionary['functions']
            for key in functions:
                val_sym = symbols(functions[key]['var'])
                val_func = functions[key]['function']
                functions[key] = (val_sym, val_func)
        
        params = RestrictionParameters(id, name, int_params_values, functions)
        restriction = Restriction(params)
        return restriction

