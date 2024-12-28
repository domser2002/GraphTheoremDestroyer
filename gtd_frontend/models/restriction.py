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
        
        params = RestrictionParameters(id, name, int_params_values, functions)
        restriction = Restriction(params)
        return restriction