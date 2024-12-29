from customtkinter import CTkFrame, CTkScrollableFrame
from customtkinter import CTkLabel
from customtkinter import CTkButton
from models.proof import Proof
import customtkinter as ctk

class ProofFrame(CTkFrame):
    def __init__(self, master, proof:Proof):
        super().__init__(master)
        self.proof:Proof = proof
        self.parent_frame_height = 150
        self.left_width = 120
        self.right_width = 150
        self.display()
        
    def display(self):

        parent_frame_height = self.parent_frame_height
        left_width = self.left_width
        right_width = self.right_width

        parent_frame = CTkFrame(
            self, 
            fg_color="#1F1F1F",
            corner_radius=15,
            height=parent_frame_height
        )

        parent_frame.pack(pady=0, padx=10, fill="x")
        parent_frame.pack_propagate(False)

        parent_frame.grid_columnconfigure(0, minsize=left_width, weight=0)
        parent_frame.grid_columnconfigure(1, weight=1)
        parent_frame.grid_columnconfigure(2, minsize=right_width, weight=0)
        parent_frame.grid_rowconfigure(0, weight=1)

        left_frame = CTkFrame(
            parent_frame, 
            #fg_color="red", 
            width=left_width, 
            fg_color="#343638",
            corner_radius=10,
            height=parent_frame_height
        )
        left_frame.grid(row=0, column=0, sticky="nsew", padx=5, pady=5)

        middle_frame = CTkScrollableFrame(
            parent_frame, 
            #fg_color="green", 
            fg_color="#343638",
            corner_radius=10,
            height=parent_frame_height
        )
        middle_frame.grid(row=0, column=1, sticky="nsew", padx=5, pady=5)

        right_frame = CTkFrame(
            parent_frame, 
            #fg_color="blue",
            fg_color="#343638",
            corner_radius=10,
            width=right_width, 
            height=parent_frame_height
        )
        right_frame.grid(row=0, column=2, sticky="nsew", padx=5, pady=5)

        self.display_general_info(left_frame)
        self.display_restrictions(middle_frame)
        self.display_result(right_frame)

    def display_general_info(self, frame: CTkFrame):
        # Header Label
        header_label = CTkLabel(
            frame,
            text="Timestamp",
            font=("Roboto", 14, "bold"),
            text_color="#00BFFF"  # DeepSkyBlue color for headers
        )
        header_label.pack(pady=(10, 5))

        # Time Label
        time_label = CTkLabel(
            frame,
            text='\n'.join(str(self.proof.timestamp).split()),
            font=("Helvetica", 16, "bold"),
            text_color="#FFFFFF",  # White text for contrast
            bg_color="transparent"
        )
        time_label.pack(expand=True, fill="both", padx=10, pady=(0, 10))

    def display_restrictions(self, frame: CTkFrame):
        header_label = CTkLabel(
            frame,
            text="Restrictions",
            font=("Roboto", 14, "bold"),
            text_color="#00BFFF"  # DeepSkyBlue color for headers
        )
        header_label.pack(pady=(10, 5))

    def display_result(self, frame: CTkFrame):
        # Header Label
        header_label = CTkLabel(
            frame,
            text="Result",
            font=("Roboto", 14, "bold"),
            text_color="#00BFFF"
        )
        header_label.pack(pady=(10, 5))

        is_success = self.proof.result == 'success'

        # Result Content (Placeholder)
        result_content = CTkLabel(
            frame,
            text="Success" if is_success else "Failure",
            font=("Helvetica", 16, "bold"),
            text_color="#32CD32" if is_success else "#FF4500",  # Green for success, OrangeRed otherwise
            justify="center"
        )
        result_content.pack(expand=True, fill="both", padx=10, pady=(0, 10))

        def test_function():
            new_window = ctk.CTkToplevel()
            new_window.title('New window')
            new_window.geometry('300x200')
            

        if is_success:
            proof_button = CTkButton(frame, text='View proof', width=self.right_width-10, command=test_function)
            proof_button.pack(padx=0, pady=5, expand=False)
            pass
