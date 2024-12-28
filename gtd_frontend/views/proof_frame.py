from customtkinter import CTkFrame
from customtkinter import CTkLabel
from models.proof import Proof

class ProofFrame(CTkFrame):
    def __init__(self, master, proof:Proof):
        super().__init__(master)
        self.proof:Proof = proof
        self.display()
        
    def display(self):
        parent_frame = CTkFrame(self, fg_color="gray")
        parent_frame.pack(pady=0, padx=0, fill="x", expand=True)

        left_width = 150
        right_width = 150

        parent_frame.grid_columnconfigure(0, minsize=left_width, weight=0)
        parent_frame.grid_columnconfigure(1, weight=1)
        parent_frame.grid_columnconfigure(2, minsize=right_width, weight=0)

        CTkLabel(parent_frame, text="Left Column", fg_color="red", width=left_width, height=100).grid(row=0, column=0, sticky="nsew", padx=5, pady=5)
        CTkLabel(parent_frame, text="Middle Column", fg_color="green").grid(row=0, column=1, sticky="nsew", padx=5, pady=5)
        CTkLabel(parent_frame, text="Right Column", fg_color="blue", width=right_width, height=100).grid(row=0, column=2, sticky="nsew", padx=5, pady=5)

        self.display_general_info()
        self.display_restrictions()
        self.display_result()

    def display_general_info(self):
        pass

    def display_restrictions(self):
        pass


    def display_result(self):
        pass
