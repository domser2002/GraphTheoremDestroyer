from customtkinter import CTkFrame, CTkLabel
from views.graph_input_view import GraphInputView
from views.restrictions_input_view import RestrictionsInputView
import os

class ProofDetailsView(CTkFrame):
    def __init__(self, master):
        super().__init__(master)

        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=50)
        self.grid_columnconfigure(1, weight=50)

        graph_save_path = os.path.join('out_data', 'start_graph.json')
        start_graph_input = GraphInputView(self, graph_save_path)
        start_graph_input.grid(row=0, column=0, sticky='nsew', padx=10, pady=(10, 5))

        restrictions_input_view = RestrictionsInputView(self)
        restrictions_input_view.grid(row=0, column=1, sticky='nsew', padx=10, pady=(10, 5))
