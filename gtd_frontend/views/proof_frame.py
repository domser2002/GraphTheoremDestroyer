from customtkinter import CTkFrame
from models.proof import Proof

class ProofFrame(CTkFrame):
    def __init__(self, master, proof:Proof):
        super().__init__(master)
        self.proof:Proof = proof
        self.display()
        
    def display(self):
        self.display_general_info()
        self.display_restrictions()
        self.display_result()

    def display_general_info(self):
        pass

    def display_restrictions(self):
        pass

    def display_result(self):
        pass
