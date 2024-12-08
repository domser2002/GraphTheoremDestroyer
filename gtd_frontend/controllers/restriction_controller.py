import json
from models.restriction import RestrictionSchema
from models.restriction import Restriction
from models.restriction import RestrictionParameters

class RestrictionController:
    def __init__(self, restr_schema_path: str, restr_output_path: str):
        self.restr_schema_path = restr_schema_path
        self.restr_output_path = restr_output_path
        self.restrictions: list[Restriction] = []
    
    def get_schemas(self):
        with open(self.restr_schema_path) as file:
            data = json.load(file)
        result = []
        for x in data:
            id = None
            name = None
            int_params = None

            if 'id' in x.keys():
                id = x['id']
            if 'name' in x.keys():
                name = x['name']
            if 'int_params' in x.keys():
                int_params = x['int_params']
            
            schema = RestrictionSchema(id, name, int_params)
            result.append(schema)
        return result
    
    def clear_restrictions(self):
        self.restrictions.clear()

    def add_restriction(self, restriction: Restriction):
        self.restrictions.append(restriction)
    
    def remove_restriction(self, idx):
        if idx < 0 or idx > len(self.restrictions):
            return
        self.restrictions = self.restrictions[:idx] + self.restrictions[idx+1:]

    def write_to_file(self):
        with open(self.restr_output_path, 'w') as file:
            datas = []
            for restr in self.restrictions:
                data = {}
                if restr.id is not None:
                    data['id'] = restr.id
                if restr.name is not None:
                    data['name'] = restr.name
                if restr.int_params_values is not None:
                    data['int_params_values'] = restr.int_params_values
                datas.append(data)
            json.dump(datas, file, indent=4)

    @staticmethod
    def create_restriction_from_params(params: RestrictionParameters):
        result = Restriction(params)
        return result
