from customtkinter import CTk
from views.main_view import MainView
from controllers.restriction_controller import RestrictionController
from controllers.proof_controller import ProofController
import os
import matplotlib.pyplot as plt
import customtkinter

class App(CTk):
    def __init__(self):
        super().__init__()
        self.title('Graph Theorem Destroyer')
        self.geometry('800x600')

        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        proof_controller = ProofController(None)
        restriction_controller = RestrictionController(proof_controller)
        main_view = MainView(self, restriction_controller, proof_controller)
        main_view.grid(row=0, column=0, sticky="nsew")

if __name__ == '__main__':

    customtkinter.set_appearance_mode("Dark")
    app = App()

    def on_close():
        plt.close('all')
        if hasattr(app, 'function_input_frame') and app.function_input_frame:
            app.function_input_frame.destroy()
        try:
            app.destroy()
        except:
            pass
    app.protocol("WM_DELETE_WINDOW", on_close)
    app.mainloop()
