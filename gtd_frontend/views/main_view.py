from customtkinter import CTkFrame, CTkTabview, CTkLabel, CTkButton
from views.proof_details_view import ProofDetailsView
from views.run_machine_view import RunMachineView
from views.proof_list_view import ProofListView
import customtkinter as ctk
import tkinter as tk

class MainView(CTkFrame):
    def __init__(self, master, restriction_controller, proof_controller):
        super().__init__(master)

        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        top_frame = CTkFrame(self)
        top_frame.grid(row=0, column=0, sticky="nsew", padx=20, pady=10)

        top_frame.grid_rowconfigure(0, weight=1)
        top_frame.grid_rowconfigure(1, weight=0)

        top_frame.grid_columnconfigure(0, weight=1)

        self.tabview = CTkTabview(top_frame)
        self.tabview.grid(row=0, column=0, sticky="nsew")

        self.tabview.add("Proof details")
        self.tabview.add("Proof in natural language")

        proof_details_tab = self.tabview.tab("Proof details")
        proof_details_tab.grid_rowconfigure(0, weight=1)
        proof_details_tab.grid_columnconfigure(0, weight=1)

        proof_details_view = ProofDetailsView(proof_details_tab, restriction_controller)
        proof_details_view.grid(row=0, column=0, sticky="nsew")

        self.button = RunMachineView(proof_details_tab, 
                                     proof_details_view.restrictions_input_view, 
                                     restriction_controller, 
                                     self)
        self.button.grid(row=1, column=0, padx=10, sticky='se')

        proof_tab = self.tabview.tab("Proof in natural language")
        proof_tab.grid_rowconfigure(0, weight=1)
        proof_tab.grid_columnconfigure(0, weight=1)

        proof_list_view = ProofListView(proof_tab, proof_controller)
        proof_controller.proof_list_view = proof_list_view
        proof_list_view.grid(row=0, column=0, sticky='nsew')
        self.proof_list_view = proof_list_view

    def switch_to_details(self):
        self.tabview.set('Proof details')
    
    def switch_to_proofs(self):
        self.tabview.set('Proof in natural language')
