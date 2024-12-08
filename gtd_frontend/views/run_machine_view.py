from customtkinter import CTkFrame, CTkLabel, CTkButton

class RunMachineView(CTkFrame):
    def __init__(self, master, restrictions_input_view):
        super().__init__(master)
        self.restrictions_input_view = restrictions_input_view

        self.run_button = CTkButton(self, 
                                    text='RUN', 
                                    fg_color=('white', 'green'),
                                    command=self.run_machine)
        self.run_button.pack(side='right')

    def run_machine(self):
        self.restrictions_input_view.write_data()