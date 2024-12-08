from customtkinter import CTkFrame, CTkLabel

class RestrictionsInputView(CTkFrame):
    def __init__(self, master):
        super().__init__(master)

        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        left_label = CTkLabel(self, text="restrictions content")
        left_label.grid(row=0, column=0, sticky="nsew", padx=10, pady=10)