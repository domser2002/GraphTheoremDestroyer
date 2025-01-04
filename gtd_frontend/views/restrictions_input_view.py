from customtkinter import CTkFrame, CTkLabel, CTkScrollableFrame
from customtkinter import CTkButton
from views.restriction_view import RestrictionView
from config import *
from models.restriction import RestrictionParameters
from controllers.restriction_controller import RestrictionController

class RestrictionsInputView(CTkScrollableFrame):
    def __init__(self, master, restriction_controller):
        super().__init__(master)
        self.restriction_schemas = restriction_controller.get_schemas()
        self.restriction_views = []
        self.new_restriction_button = None

        self.add_new_restriction_button()
        

    def add_blank_restriction(self):
        restr_view = RestrictionView(self, self.restriction_schemas, self)
        restr_view.display()
        self.destroy_new_restriction_button()
        restr_view.pack(fill='x', pady=5)
        self.add_new_restriction_button()
        self.restriction_views.append(restr_view)
    
    def add_new_restriction_button(self):
        self.new_restriction_button = CTkButton(self, 
                                                text='Add new restriction',
                                                command=self.add_blank_restriction)
        self.new_restriction_button.pack(pady=10)
    
    def kill_view(self, view: RestrictionView):
        if view not in self.restriction_views:
            return
        self.restriction_views.remove(view)
        view.destroy()

    def destroy_new_restriction_button(self):
        self.new_restriction_button.destroy()
    
    def get_restriction_objects(self):
        result = []
        for restr_view in self.restriction_views:
            restr_obj = restr_view.get_restriction_object()
            result.append(restr_obj)
        return result
    