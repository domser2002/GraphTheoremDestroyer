from customtkinter import CTkFrame, CTkTabview, CTkLabel, CTkButton
from views.proof_details_view import ProofDetailsView
from views.run_machine_view import RunMachineView

class MainView(CTkFrame):
    def __init__(self, master, restriction_controller):
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

        self.button = RunMachineView(top_frame, proof_details_view.restrictions_input_view, restriction_controller)
        self.button.grid(row=1, column=0, padx=10)

        natural_language_tab = self.tabview.tab("Proof in natural language")
        label2 = CTkLabel(natural_language_tab, text="Natural language explanation")
        label2.pack(pady=10, padx=10)
