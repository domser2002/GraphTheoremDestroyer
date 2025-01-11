import json
import os
import asyncio
from models.restriction import RestrictionSchema, Restriction, RestrictionParameters
import threading
import socket
import time
import random
from models.response import *
from controllers.proof_controller import ProofController
from models.proof import Proof
import datetime
from config import *

class RestrictionController:
    def __init__(self, proof_controller: ProofController):
        self.restrictions: list[Restriction] = []
        self.proof_controller: ProofController = proof_controller
        self.server_address = ('localhost', 8080)  # Assuming localhost and PORT 8080
        self.sock = None
        self.init_controller()

    def init_controller(self):
        '''
        Initialize the connection to the backend server.
        '''
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect(self.server_address)
            print("Connected to backend server.")
        except socket.error as e:
            print(f"Socket error: {e}")
            self.sock = None

    def get_schemas(self):
        '''
        Receive and parse the JSON file path with supported restrictions from the backend.
        '''
        if self.sock:
            try:
                restr_schema_path = self.sock.recv(1024).decode('utf-8')
                print(f"Received restrictions path: {restr_schema_path}")
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
            except Exception as e:
                print(f"Error receiving schemas: {e}")
        return []

    def get_backend_response(self):
        '''
        Send selected restrictions to the backend and wait for a response.
        '''
        current_datetime = datetime.datetime.now().strftime('%Y%m%d%H%M%S')

        restr_output_path = os.path.abspath(
            os.path.join('..', 'metadata', f'restrictions_out_{current_datetime}.json')
        )
        if self.sock:
            try:
                with open(restr_output_path, 'w') as file:
                    datas = [self._restriction_to_dict(restr) for restr in self.restrictions]
                    json.dump(datas, file, indent=4)

                # Send the path to the backend
                self.sock.sendall(restr_output_path.encode('utf-8'))
                print(f"Sent restrictions path: {restr_output_path}")

                # Wait for the backend response
                data = self.sock.recv(1024).decode('utf-8')
                print(f"Received result path: {data}")

                response = Response(ResponseResult.SUCCESS, data)
                return response

            except Exception as e:
                print(f"Error communicating with backend: {e}")
                return Response(ResponseResult.FAILURE, "")

        return Response(ResponseResult.FAILURE, "")

    def _restriction_to_dict(self, restr):
        data = {}
        if restr.id is not None:
            data['id'] = restr.id
        if restr.name is not None:
            data['name'] = restr.name
        if restr.int_params_values is not None and len(restr.int_params_values) > 0:
            data['int_params_values'] = restr.int_params_values
        if restr.functions is not None and len(restr.functions) > 0:
            data['functions'] = restr.functions
        return data

    def close_connection(self):
        '''
        Close the socket connection to the backend server.
        '''
        if self.sock:
            self.sock.close()
            self.sock = None
            print("Closed connection to backend server.")

    
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
