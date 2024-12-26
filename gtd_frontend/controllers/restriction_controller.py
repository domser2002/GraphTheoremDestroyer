import json
from models.restriction import RestrictionSchema
from models.restriction import Restriction
from models.restriction import RestrictionParameters
import os

class RestrictionController:
    def __init__(self):
        self.restrictions: list[Restriction] = []
        self.init_controller()
    
    def init_controller(self):
        '''
        override this function
        use it to make necessary connections to the backend
        if you want to, you can create separate controller
        '''
        pass
    
    def get_schemas(self):
        '''
        override this function
        use it to return restriction schemas
        to be more specific, list of RestrictionSchema objects
        '''
        restr_schema_path = os.path.join('input_data', 'restrictions.json')
        with open(restr_schema_path) as file:
            data = json.load(file)
        result = []
        for x in data:
            id = None
            name = None
            int_params = []
            function = None

            if 'id' in x.keys():
                id = x['id']
            if 'name' in x.keys():
                name = x['name']
            if 'int_params' in x.keys():
                int_params = x['int_params']
            if 'function' in x.keys():
                function = x['function']
            
            schema = RestrictionSchema(id, name, int_params, function)
            result.append(schema)
        return result
    
    def send_restriction_data(self):
        '''
        override this function
        use it to send info about chosen restrictions to the backend
        '''
        restr_output_path = os.path.join('out_data', 'restrictions_out.json')
        with open(restr_output_path, 'w') as file:
            datas = []
            for restr in self.restrictions:
                data = {}
                if restr.id is not None:
                    data['id'] = restr.id
                if restr.name is not None:
                    data['name'] = restr.name
                if restr.int_params_values is not None and len(restr.int_params_values) > 0:
                    data['int_params_values'] = restr.int_params_values
                if restr.function is not None:
                    data['function'] = restr.function
                datas.append(data)
            json.dump(datas, file, indent=4)
    
    def clear_restrictions(self):
        self.restrictions.clear()

    def add_restriction(self, restriction: Restriction):
        self.restrictions.append(restriction)
    
    def remove_restriction(self, idx):
        if idx < 0 or idx > len(self.restrictions):
            return
        self.restrictions = self.restrictions[:idx] + self.restrictions[idx+1:]


    @staticmethod
    def create_restriction_from_params(params: RestrictionParameters):
        result = Restriction(params)
        return result
