from customtkinter import CTkFrame, CTkLabel, CTkButton

class RunMachineView(CTkFrame):
    def __init__(self, master, 
                restrictions_input_view,
                restriction_controller,
                proof_controller):
        super().__init__(master)
        self.restrictions_input_view = restrictions_input_view
        self.restriction_controller = restriction_controller

        self.run_button = CTkButton(self, 
                                    text='RUN', 
                                    fg_color=('white', 'green'),
                                    command=self.run_machine)
        self.run_button.pack(side='right')

    def send_restrictions(self):
        restrictions = self.restrictions_input_view.get_restriction_objects()
        self.restriction_controller.clear_restrictions()
        for restr in restrictions:
            self.restriction_controller.add_restriction(restr)
        self.restriction_controller.send_restriction_data()

    def run_machine(self):
        self.send_restrictions()