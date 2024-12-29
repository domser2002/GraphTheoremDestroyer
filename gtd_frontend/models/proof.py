from models.restriction import Restriction
import datetime
import os
import json

class Proof:
    
    def __init__(self):
        self.result = None
        self.text:list[str] = None
        self.restrictions:list[Restriction] = None
        self.timestamp:datetime.datetime = None
    
    def save_to_json(self, folder_path: str, proof_path: str):
        idx = 0
        file_path = os.path.join(folder_path, f'proof{idx}.json')
        while os.path.exists(file_path):
            idx += 1
            file_path = os.path.join(folder_path, f'proof{idx}.json')
        result = {}
        result['result'] = self.result
        result['proof_path'] = proof_path
        result['timestamp'] = self.timestamp.strftime('%Y-%m-%dT%H:%M')
        result['restrictions'] = []
        for restr in self.restrictions:
            result['restrictions'].append(restr.to_dictionary())
        with open(file_path, 'w') as file:
            json.dump([result], file, indent=4, ensure_ascii=False, sort_keys=False)
    
    @staticmethod
    def from_dictionary(dictionary: dict):
        result = dictionary['result']
        proof_text = []
        if result.lower() == 'success':
            proof_path = dictionary['proof_path']
            with open(proof_path, 'r') as file:
                proof_text = file.readlines()
                proof_text = [x.rstrip() for x in proof_text]
        restrictions = []
        restrictions_raw = dictionary['restrictions']
        timestamp = dictionary['timestamp']
        timestamp = datetime.datetime.strptime(timestamp, "%Y-%m-%dT%H:%M")
        for restriction in restrictions_raw:
            restr = Restriction.from_dictionary(restriction)
            restrictions.append(restr)
        
        proof = Proof()
        proof.result = result
        proof.text = proof_text
        proof.restrictions = restrictions
        proof.timestamp = timestamp

        return proof

