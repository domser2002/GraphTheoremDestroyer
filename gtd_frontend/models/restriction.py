class RestrictionSchema:

    def __init__(self, id, name, int_params):
        self.id = id
        self.name = name
        self.int_params = int_params

    def __repr__(self):
        return f'RestrictionSchema(id: {self.id}, name: {self.name}, int_params: {self.int_params})\n'

class RestrictionParameters:
    def __init__(self, 
                 restriction_id: int,
                 restriction_name: str,
                 int_params_values: list[int]):
        
        self.int_params_values = int_params_values
        self.restriction_id = restriction_id
        self.restriction_name = restriction_name
    
class Restriction:

    def __init__(self, params: RestrictionParameters):
        id = params.restriction_id
        name = params.restriction_name
        int_params_values = params.int_params_values

        self.id = id
        self.name = name
        self.int_params_values = int_params_values