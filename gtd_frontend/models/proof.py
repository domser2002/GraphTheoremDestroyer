from models.restriction import Restriction
import datetime

class Proof:
    
    def __init__(self):
        self.result = None
        self.text:list[str] = None
        self.restrictions:list[Restriction] = None
        self.timestamp:datetime.datetime = None
    
    @staticmethod
    def from_dictionary(dictionary: dict):
        result = dictionary['result']
        proof_path = dictionary['proof_path']
        restrictions = []
        restrictions_raw = dictionary['restrictions']
        timestamp = dictionary['timestamp']
        timestamp = datetime.datetime.strptime(timestamp, "%Y-%m-%dT%H:%M")
        for restriction in restrictions_raw:
            restr = Restriction.from_dictionary(restriction)
            restrictions.append(restr)
        
        proof = Proof()
        proof.result = result
        proof.text = ""
        proof.restrictions = restrictions
        proof.timestamp = timestamp

        return proof

