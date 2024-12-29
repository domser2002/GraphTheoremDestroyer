from models.proof import Proof
import os
from config import *
import json

class ProofController:

    def __init__(self):
        self.proofs:list[Proof] = []
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
                self.add_proof(proof)
        
    def add_proof(self, proof:Proof):
        self.proofs.append(proof)
        self.proofs.sort(key=lambda x:x.timestamp, reverse=True)
    
    def get_proofs(self):
        proofs = self.proofs
        return proofs