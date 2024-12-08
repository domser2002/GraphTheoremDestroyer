from customtkinter import CTkFrame, CTkLabel, CTkScrollableFrame
from customtkinter import CTkButton
from views.restriction_view import RestrictionView
from config import *
from models.restriction import RestrictionParameters
from controllers.restriction_controller import RestrictionController

class RestrictionsInputView(CTkScrollableFrame):
    def __init__(self, master, restriction_controller):
        super().__init__(master)
        self.restriction_controller = restriction_controller
        self.restr_schemas = self.restriction_controller.get_schemas()
        self.restr_views = []

        # for i in range(3):
        #     self.add_blank_restriction()
        
        self.place_add_button()
    
    def add_blank_restriction(self):

        self.destroy_add_button()

        restr = RestrictionView(self, self.restr_schemas, BLANK_OPTION)
        self.restr_views.append(restr)
        restr.pack(pady=10, fill='x')

        self.place_add_button()
    
    def destroy_add_button(self):
        self.add_button.destroy()

    def place_add_button(self):
        self.add_button = CTkButton(self, text='Add blank restriction',
                                    command=self.add_blank_restriction)
        self.add_button.pack()
    
    def write_data(self):
        self.restriction_controller.clear_restrictions()
        for restr_view in self.restr_views:
            id = None
            name = None
            int_params_values = None
            data = restr_view.data
            print(data)
            if 'id' in data.keys():
                id = data['id']
            if 'name' in data.keys():
                name = data['name']
            for schema in self.restr_schemas:
                if schema.name == name:
                    if schema.int_params is None:
                        continue
                    for key in data.keys():
                        if key in schema.int_params:
                            val = data[key]
                            if int_params_values is None:
                                int_params_values = [val]
                            else:
                                int_params_values.append(val)
                    break

            if 'int_params_values' in data.keys():
                int_params_values = data['int_params_values']
            params = RestrictionParameters(id, name, int_params_values)
            restr = RestrictionController.create_restriction_from_params(params)
            self.restriction_controller.add_restriction(restr)
        self.restriction_controller.write_to_file()

    