from customtkinter import CTkFrame, CTkLabel, CTkButton, CTkComboBox, CTkEntry
from models.restriction import RestrictionParameters, RestrictionSchema, Restriction
import tkinter as tk
from config import *
from views.function_input_frame import FunctionInputFrame

class RestrictionView(CTkFrame):
    def __init__(self, master, restr_schemas: list[RestrictionSchema], restrictions_input_view):
        super().__init__(master)
        # self.configure(fg_color='blue')
        self.pack_propagate(False)  # Prevent the frame from shrinking to fit its contents
        self.configure(height=RESTRICTION_BOX_HEIGHT)
        
        default_schema = RestrictionSchema(None, RESTRICTION_COMBO_BOX_DEFAULT_OPTION, [], [])
        self.restriction_schemas = [default_schema] + restr_schemas
        self.restriction_params: RestrictionParameters = RestrictionParameters(None, RESTRICTION_COMBO_BOX_DEFAULT_OPTION, {}, {})
        self.chosen_schema = self.restriction_schemas[0]
        self.options = [x.name for x in self.restriction_schemas]
        self.left_width = RESTRICTION_COMBO_BOX_WIDTH
        self.left_frame = None
        self.right_frame = None
        self.restrictions_input_view = restrictions_input_view

    def clear_params(self):
        self.restriction_params.restriction_id = None
        self.restriction_params.restriction_name = None
        self.restriction_params.int_params_values = {}

    def clear_frame(self):
        for widget in self.winfo_children():
            widget.destroy()
        self.left_frame = None
        self.right_frame = None

    def display(self):
        self.clear_frame()
        self.display_frames()
        self.display_combo_box()
        self.display_int_input_fields()
        self.display_function_input_field()
        self.display_delete_button()

    def display_frames(self):
        if self.left_frame is None or self.right_frame is None:
            parent_frame = CTkFrame(
                self,
                fg_color='#1F1F1F',
                corner_radius=15,
                height=RESTRICTION_BOX_HEIGHT
            )
            parent_frame.pack(pady=0, padx=0, fill='x')
            parent_frame.pack_propagate(False)
            parent_frame.grid_columnconfigure(0, minsize=self.left_width, weight=0)
            parent_frame.grid_columnconfigure(1, weight=1)
            parent_frame.grid_rowconfigure(0, weight=1)

            self.left_frame = CTkFrame(
                parent_frame,
                width=self.left_width,
                fg_color="#343638",
                corner_radius=10,
                height=RESTRICTION_BOX_HEIGHT
            )
            self.left_frame.grid(row=0, column=0, sticky="nsew", padx=5, pady=5)

            self.right_frame = CTkFrame(
                parent_frame,
                fg_color='#343638',
                corner_radius=10,
                height=RESTRICTION_BOX_HEIGHT
            )
            self.right_frame.grid(row=0, column=1, sticky="nsew", padx=5, pady=5)
            self.right_frame.pack_propagate(False)


    def display_combo_box(self):
        combo_box = CTkComboBox(self.left_frame, values=self.options, command=self.select_choice, width=RESTRICTION_COMBO_BOX_WIDTH)
        combo_box.pack(side='left')
        combo_box.set(self.restriction_params.restriction_name)

    def display_int_input_fields(self):

        def is_digit_or_empty(P):
            return len(P) == 0 or P.isdigit()
        
        val_command = (self.right_frame.register(is_digit_or_empty), '%P')

        for int_field_label in self.chosen_schema.int_params:
            label = CTkLabel(self.right_frame, text=f'{int_field_label}:')
            # label.pack_propagate(False)
            label.pack(side='left', padx=5)
            if int_field_label in self.restriction_params.int_params_values.keys():
                int_var = self.restriction_params.int_params_values[int_field_label]
            else:
                int_var = tk.StringVar(value=str(INT_ENTRY_FIELD_DEFAULT))
                self.restriction_params.int_params_values[int_field_label] = int_var
            entry = CTkEntry(self.right_frame,
                              width=50, 
                              textvariable=int_var,
                              validate='key',
                              validatecommand=val_command)
            entry.pack(side='left', padx=10)
            
    
    def display_function_input_field(self):
        for function_name in self.chosen_schema.functions:
                label = CTkLabel(self.right_frame, text=f'{function_name}:')
                label.pack(side='left', padx=5)
                function_frame = FunctionInputFrame(self.right_frame)
                self.restriction_params.functions[function_name] = function_frame
                function_frame.pack(side='left', padx=5)
    

    def display_delete_button(self):
        size = 30
        def suicide():
            self.restrictions_input_view.kill_view(self)
        button = CTkButton(
                    master=self.right_frame,
                    text="✕",
                    width=size,
                    height=size,
                    font=("Roboto", 20, "bold"),
                    text_color="white",
                    fg_color="#e94560",
                    hover_color="#ff6b81",
                    corner_radius=5,
                    command=suicide
                )
        button.pack(side='right', padx=10, pady=5)


    def select_choice(self, choice):
        if choice == self.restriction_params.restriction_name:
            return
        self.clear_params()
        self.chosen_schema = [x for x in self.restriction_schemas if x.name == choice][0]
        self.restriction_params.restriction_id = self.chosen_schema.id
        self.restriction_params.restriction_name = self.chosen_schema.name
        self.update_input_fields()
    
    def update_input_fields(self):
        for widget in self.right_frame.winfo_children():
            widget.destroy()

        self.display_int_input_fields()
        self.display_function_input_field()
        self.display_delete_button()


    def get_restriction_object(self):
        id = self.restriction_params.restriction_id
        name = self.restriction_params.restriction_name
        int_params_values = {}
        for key in self.restriction_params.int_params_values.keys():
            value = self.restriction_params.int_params_values[key].get()
            int_params_values[key] = value
        function_values = {}
        for key in self.restriction_params.functions.keys():
            value = self.restriction_params.functions[key].get_value()
            function_values[key] = value
        params = RestrictionParameters(id, name, int_params_values, function_values)
        result = Restriction(params)
        return result