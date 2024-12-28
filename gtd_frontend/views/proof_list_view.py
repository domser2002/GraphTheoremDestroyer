from customtkinter import CTkFrame
from customtkinter import CTkLabel
from models.proof import Proof
from controllers.proof_controller import ProofController
from views.proof_frame import ProofFrame

class ProofListView(CTkFrame):
    def __init__(self, master, proof_controller: ProofController):
        super().__init__(master)
        self.proof_views:list[ProofFrame] = []
        self.proof_controller:ProofController = proof_controller
        self.init_proofs()

    def init_proofs(self):
        proofs = self.proof_controller.get_proofs()
        for proof in proofs:
            self.add_proof_frame(proof)
    
    def add_proof_frame(self, proof:Proof):
        proof_frame = ProofFrame(self, proof)
        self.proof_views.append(proof_frame)
        proof_frame.pack()