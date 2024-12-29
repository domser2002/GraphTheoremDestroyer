from customtkinter import CTkFrame, CTkScrollableFrame
from customtkinter import CTkLabel
from customtkinter import CTkButton
from models.proof import Proof
from views.proof_text_frame import ProofTextFrame
from models.restriction import Restriction
import customtkinter as ctk
from sympy import symbols, Add, Rational, Poly, sympify, latex
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

class ProofFrame(CTkFrame):
    def __init__(self, master, proof:Proof):
        super().__init__(master)
        self.proof:Proof = proof
        self.parent_frame_height = 150
        self.left_width = 120
        self.right_width = 150
        self.left_frame = None
        self.middle_frame = None
        self.right_frame = None
        self.right_frame_to_destroy = []
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
        self.left_frame = left_frame

        middle_frame = CTkScrollableFrame(
            parent_frame, 
            #fg_color="green", 
            fg_color="#343638",
            corner_radius=10,
            height=parent_frame_height
        )
        middle_frame.grid(row=0, column=1, sticky="nsew", padx=5, pady=5)
        self.middle_frame = middle_frame

        right_frame = CTkFrame(
            parent_frame, 
            #fg_color="blue",
            fg_color="#343638",
            corner_radius=10,
            width=right_width, 
            height=parent_frame_height
        )
        right_frame.grid(row=0, column=2, sticky="nsew", padx=5, pady=5)
        self.right_frame = right_frame

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
        # header_label.grid(row=0, column=0)

        for idx, restr in enumerate(self.proof.restrictions):
            self.display_restriction(frame, restr, idx+1)
    
    def display_restriction(self, frame: CTkScrollableFrame, restriction: Restriction, row_idx: int):
        restr_frame = CTkFrame(frame, bg_color='transparent', fg_color='white', height=20, corner_radius=5)
        # restr_frame.grid(row=row_idx, column=0, sticky='w', padx=5, pady=5)
        restr_frame.pack(fill='both', expand=True, pady=5)

        # display name
        name = restriction.name
        header_label = CTkLabel(
            restr_frame,
            text=name,
            font=('Roboto', 14, 'bold'),
            bg_color='transparent',
            text_color='black'
        )
        header_label.grid(row=0, column=0, sticky='w', padx=10)
        idx = 1

        # display int params
        int_params_values = restriction.int_params_values
        for key, val in [(k, int_params_values[k]) for k in int_params_values]:
            int_frame = CTkFrame(
                restr_frame,
                bg_color='transparent',
                fg_color='transparent'
                )

            key_label = CTkLabel(
                int_frame,
                text=f'{key}:',
                font=('Roboto', 14, 'bold'),
                bg_color='transparent',
                text_color='black'
            )
            key_label.grid(row=0, column=0, padx=3, sticky='w')

            val_label = CTkLabel(
                int_frame,
                text=f'{val}',
                font=('Roboto', 14, 'italic'),
                bg_color='transparent',
                text_color='black'
            )
            val_label.grid(row=0, column=1, padx=3, sticky='w')

            int_frame.grid(row=idx, column=0, sticky='w')
            idx += 1
        
        # display function params
        functions = restriction.functions
        for key, val in [(k, functions[k]) for k in functions]:
            # print(key, val)
            func_frame = CTkFrame(
                restr_frame,
                bg_color='transparent',
                fg_color='transparent'
                )

            key_label = CTkLabel(
                func_frame,
                text=f'{key}:',
                font=('Roboto', 14, 'bold'),
                bg_color='transparent',
                text_color='black'
            )
            key_label.grid(row=0, column=0, padx=3, sticky='w')

            # TODO make proper positioning
            x = symbols('x')
            terms = []
            for n, d, p in val:
                terms.append(Rational(n, d) * x**p)
            poly = Poly(Add(*terms), x)
            poly = Add(*terms)
            latex_expr = latex(sympify(poly))
            fig = plt.Figure(figsize=(5, 1))
            canvas = FigureCanvasTkAgg(fig, master=func_frame)
            canvas_widget = canvas.get_tk_widget()
            canvas_widget.grid(row=0, column=1, padx=0, sticky='w')
            fig.clear()
            ax = fig.add_subplot(111)
            ax.text(0, 0, f"${latex_expr}$")
            ax.axis('off')

            func_frame.grid(row=idx, column=0, sticky='w')
            idx += 1

    def display_result(self, frame: CTkFrame):
        # Header Label
        header_label = CTkLabel(
            frame,
            text="Result",
            font=("Roboto", 14, "bold"),
            text_color="#00BFFF"
        )
        header_label.pack(pady=(10, 5))
        self.right_frame_to_destroy.append(header_label)

        is_success = self.proof.result.lower() == 'success'
        is_failure = self.proof.result.lower() == 'failure'

        # Result Content (Placeholder)
        result_content = CTkLabel(
            frame,
            text="Success" if is_success else "Failure" if is_failure else "Pending",
            font=("Helvetica", 16, "bold"),
            text_color="#32CD32" if is_success else "#FF4500" if is_failure else 'gray',
            justify="center"
        )
        result_content.pack(expand=True, fill="both", padx=10, pady=(0, 10))
        self.right_frame_to_destroy.append(result_content)

        def display_proof_window():
            new_window = ctk.CTkToplevel()
            proof_text_frame = ProofTextFrame(new_window, self.proof)
            #proof_text_frame.grid(row=0, column=0, sticky='nsew', padx=5, pady=5)
            proof_text_frame.pack(expand=True, fill='both', padx=5, pady=5)
            new_window.title('New window')
            new_window.geometry('300x200')
            

        if is_success:
            proof_button = CTkButton(frame, text='View proof', width=self.right_width-10, command=display_proof_window)
            proof_button.pack(padx=0, pady=5, expand=False)
            self.right_frame_to_destroy.append(proof_button)
    
    def redisplay_result(self):
        # for widget in self.right_frame.winfo_children():
        #     if not isinstance(widget, (ctk.CTkBaseClass,)):
        #         widget.destroy()
        for widget in self.right_frame_to_destroy:
            widget.destroy()
        self.display_result(self.right_frame)
