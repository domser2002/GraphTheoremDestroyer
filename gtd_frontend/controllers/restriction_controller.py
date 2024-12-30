import json
import os
import asyncio
from models.restriction import RestrictionSchema, Restriction, RestrictionParameters
import threading
import time
import random
from models.response import *
from controllers.proof_controller import ProofController
from models.proof import Proof
import datetime
from config import *

class RestrictionController:
    def __init__(self, proof_controller:ProofController):
        self.restrictions: list[Restriction] = []
        self.proof_controller:ProofController = proof_controller
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
            functions = []

            if 'id' in x.keys():
                id = x['id']
            if 'name' in x.keys():
                name = x['name']
            if 'int_params' in x.keys():
                int_params = x['int_params']
            if 'functions' in x.keys():
                functions = x['functions']
            
            schema = RestrictionSchema(id, name, int_params, functions)
            result.append(schema)
        return result
    
    
    def get_backend_response(self):
        '''
        override the code below
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
                if restr.functions is not None and len(restr.functions) > 0:
                    data['functions'] = restr.functions
                datas.append(data)
            json.dump(datas, file, indent=4)
    
        '''
        override the code below
        use it to wait for a response from backend
        a response should be a Response object (check Response model)
        '''
        # simulate response from backend
        # and wait for the incoming data
        time.sleep(3)
        response = Response(ResponseResult.FAILURE, "")
        if random.randint(0, 1) == 1:
            response.result = ResponseResult.SUCCESS
            with open(os.path.join('proof_data', 'example.txt'), 'w') as file:
                file.write('A = B\n')
                file.write('B = C\n')
                file.write('if A = B and B = C, then A = C')
            response.proof_path = os.path.join('proof_data', 'example.txt')

        return response
    
    def send_restriction_data(self):
        # wait for a data in a separate thread
        threading.Thread(target=self.wait_for_result, args=(self.restrictions,), daemon=True).start()
    
    def wait_for_result(self, restrictions:list[Restriction]):
        # add pending proof to the proof controller
        proof = Proof()
        proof.result = 'Pending'
        proof.restrictions = restrictions
        proof.timestamp = datetime.datetime.now()
        self.proof_controller.add_pending_proof(proof)

        response = self.get_backend_response()

        # modify proof
        if response.result == ResponseResult.SUCCESS:
            proof.result = 'success'
            proof_path = response.proof_path
            with open(proof_path, 'r') as file:
                proof_text = file.readlines()
                proof_text = [x.rstrip() for x in proof_text]
            proof.text = proof_text
        else:
            proof.result = "failure"
        
        # update user interface
        self.proof_controller.update_proof_result(proof)

        # save to history
        proof.save_to_json(PROOF_DATA_PATH, response.proof_path)
        

    def clear_restrictions(self):
        self.restrictions.clear()

    def add_restriction(self, restriction: Restriction):
        self.restrictions.append(restriction)
    
    def remove_restriction(self, idx):
        if idx < 0 or idx >= len(self.restrictions):
            return
        self.restrictions = self.restrictions[:idx] + self.restrictions[idx+1:]

    @staticmethod
    def create_restriction_from_params(params: RestrictionParameters):
        result = Restriction(params)
        return result
