from customtkinter import CTkScrollableFrame
from customtkinter import CTkLabel
from models.proof import Proof
from controllers.proof_controller import ProofController
from views.proof_frame import ProofFrame

class ProofListView(CTkScrollableFrame):
    def __init__(self, master, proof_controller: ProofController):
        super().__init__(master)
        self.proof_views: list[ProofFrame] = []
        self.proof_controller: ProofController = proof_controller

        self.grid_columnconfigure(0, weight=1)

        self.init_proofs()

    def init_proofs(self):
        proofs = self.proof_controller.get_proofs()
        for proof in proofs:
            self.add_proof_frame(proof)

    def add_proof_frame(self, proof: Proof):
        proof_frame = ProofFrame(self, proof)
        self.proof_views.append(proof_frame)
        proof_frame.grid(row=len(self.proof_views) - 1, column=0, sticky="ew", pady=10)

    def update_proof_result(self, proof: Proof):
        for proof_view in self.proof_views:
            if proof_view.proof is not proof:
                continue
            proof_view.redisplay_result()