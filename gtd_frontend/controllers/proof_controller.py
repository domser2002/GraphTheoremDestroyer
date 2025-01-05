from models.proof import Proof
import os
from config import *
import json

class ProofController:

    def __init__(self, proof_list_view):
        self.proofs:list[Proof] = []
        self.proof_list_view = proof_list_view
        self.read_proofs()
        pass

    def read_proofs(self):
        if not os.path.isdir(PROOF_DATA_PATH):
            os.mkdir(PROOF_DATA_PATH)
        
        file_names = [x for x in os.listdir(PROOF_DATA_PATH) if x.endswith('.json')]
        for file_name in file_names:
            file_path = os.path.join(PROOF_DATA_PATH, file_name)
            with open(file_path, 'r') as file:
                data = json.load(file)[0]
                proof = Proof.from_dictionary(data)
                proof.json_path = file_path
                self.add_proof(proof)
        
    def add_proof(self, proof:Proof):
        self.proofs.append(proof)
        self.proofs.sort(key=lambda x:x.timestamp, reverse=True)
    
    def add_pending_proof(self, proof:Proof):
        self.add_proof(proof)
        self.proof_list_view.add_proof_frame(proof)
    
    def update_proof_result(self, proof:Proof):
        self.proof_list_view.update_proof_result(proof)
    
    def get_proofs(self):
        proofs = self.proofs
        return proofs