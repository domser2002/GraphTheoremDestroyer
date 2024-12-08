from customtkinter import CTkFrame
from customtkinter import CTkLabel
from customtkinter import CTkButton
from customtkinter import CTkComboBox
from customtkinter import CTkEntry
from config import *

class RestrictionView(CTkFrame):
    def __init__(self, master, restr_schemas, option):
        super().__init__(master, height=RESTRICTION_BOX_HEIGHT)
        self.restr_schemas = restr_schemas
        self.options = [BLANK_OPTION] + [x.name for x in self.restr_schemas]
        self.option = option
        self.last_option = option
        self.combo_box = CTkComboBox(self, values=self.options, command=self.set_option)
        self.combo_box.pack(side='left')
        self.widgets = []
        self.data = {}
        self.display()
    
    def display(self):
        if self.last_option == self.option:
            return
        self.clear_frame()

        if self.option == BLANK_OPTION:
            return
        
        schema = [x for x in self.restr_schemas if x.name == self.option][0]
        if schema.int_params != None:
            for param_name in schema.int_params:
                label = CTkLabel(self, text=f'{param_name}:')
                label.pack(side='left', padx=10)
                self.widgets.append(label)

                entry = CTkEntry(self, width=50)
                param_name_cp = param_name
                entry_cp = entry
                entry.bind('<Return>', lambda event, key=param_name_cp, ent=entry_cp: self.modify_data(key, ent))
                entry.pack(side='left')
                self.widgets.append(entry)
    
    def modify_data(self, key, entry):
        self.data[key] = entry.get()
        print(self.data)

    def set_option(self, option):
        self.last_option = self.option
        self.option = option
        if option == BLANK_OPTION:
            return
        self.data['name'] = self.option
        print(self.data)
        self.display()

    def clear_frame(self):
        for widget in self.widgets:
            widget.destroy()
        if 'name' in self.data.keys():
            self.data = {'name': self.data['name']}
        else:
            self.data = {}
        # self.configure(height=RESTRICTION_BOX_HEIGHT)
        # self.update()
