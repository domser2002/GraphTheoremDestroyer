from customtkinter import CTkFrame, CTkScrollableFrame
from customtkinter import CTkLabel
from models.proof import Proof

class ProofTextFrame(CTkScrollableFrame):
    def __init__(self, master, proof:Proof):
        super().__init__(master)
        self.proof = proof
        self.display_proof()
    
    def display_proof(self):
        self.columnconfigure(0, weight=1)
        for idx, line in enumerate(self.proof.text):
            line_label = CTkLabel(self, text=line, fg_color='transparent', bg_color='transparent')
            line_label.grid(row=idx, column=0, sticky='w')