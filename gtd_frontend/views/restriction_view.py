from customtkinter import CTkFrame, CTkLabel, CTkButton, CTkComboBox, CTkEntry
from models.restriction import RestrictionParameters, RestrictionSchema, Restriction
import tkinter as tk
from config import *
from views.function_input_frame import FunctionInputFrame

class RestrictionView(CTkFrame):
    def __init__(self, master, restr_schemas: list[RestrictionSchema]):
        super().__init__(master)
        # self.configure(fg_color='blue')
        self.pack_propagate(False)  # Prevent the frame from shrinking to fit its contents
        self.configure(height=RESTRICTION_BOX_HEIGHT)
        
        default_schema = RestrictionSchema(None, RESTRICTION_COMBO_BOX_DEFAULT_OPTION, [], None)
        self.restriction_schemas = [default_schema] + restr_schemas
        self.restriction_params: RestrictionParameters = RestrictionParameters(None, RESTRICTION_COMBO_BOX_DEFAULT_OPTION, {}, None)
        self.chosen_schema = self.restriction_schemas[0]
        self.options = [x.name for x in self.restriction_schemas]

    def clear_params(self):
        self.restriction_params.restriction_id = None
        self.restriction_params.restriction_name = None
        self.restriction_params.int_params_values = {}

    def clear_frame(self):
        for widget in self.winfo_children():
            widget.destroy()

    def display(self):
        self.clear_frame()
        self.display_combo_box()
        self.display_int_input_fields()
        self.display_function_input_field()

    def display_combo_box(self):
        combo_box = CTkComboBox(self, values=self.options, command=self.select_choice, width=RESTRICTION_COMBO_BOX_WIDTH)
        combo_box.pack(side='left')
        combo_box.set(self.restriction_params.restriction_name)

    def display_int_input_fields(self):
        for int_field_label in self.chosen_schema.int_params:
            label = CTkLabel(self, text=f'{int_field_label}:')
            label.pack(side='left', padx=5)
            if int_field_label in self.restriction_params.int_params_values.keys():
                int_var = self.restriction_params.int_params_values[int_field_label]
            else:
                int_var = tk.StringVar(value=str(INT_ENTRY_FIELD_DEFAULT))
                self.restriction_params.int_params_values[int_field_label] = int_var
            entry = CTkEntry(self, width=50, textvariable=int_var)
            entry.pack(side='left', padx=10)
    
    def display_function_input_field(self):
        if self.chosen_schema.function is not None:
            label = CTkLabel(self, text=f'{self.chosen_schema.function}:')
            label.pack(side='left', padx=5)
            self.restriction_params.function = FunctionInputFrame(self)
            self.restriction_params.function.pack(side='left', padx=5)

    def select_choice(self, choice):
        if choice == self.restriction_params.restriction_name:
            return
        self.clear_params()
        self.chosen_schema = [x for x in self.restriction_schemas if x.name == choice][0]
        self.restriction_params.restriction_id = self.chosen_schema.id
        self.restriction_params.restriction_name = self.chosen_schema.name
        self.display()

    def get_restriction_object(self):
        id = self.restriction_params.restriction_id
        name = self.restriction_params.restriction_name
        int_params_values = {}
        for key in self.restriction_params.int_params_values.keys():
            value = self.restriction_params.int_params_values[key].get()
            int_params_values[key] = value
        function = None
        if type(self.restriction_params.function) is FunctionInputFrame:
            function = self.restriction_params.function.get_value()
        params = RestrictionParameters(id, name, int_params_values, function)
        result = Restriction(params)
        return result