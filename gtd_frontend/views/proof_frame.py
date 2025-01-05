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
import time

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
        self.result_label = None
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
            width=left_width, 
            fg_color="#343638",
            corner_radius=10,
            height=parent_frame_height
        )
        left_frame.grid(row=0, column=0, sticky="nsew", padx=5, pady=5)
        self.left_frame = left_frame

        middle_frame = CTkScrollableFrame(
            parent_frame, 
            fg_color="#343638",
            corner_radius=10,
            height=parent_frame_height
        )
        middle_frame.grid(row=0, column=1, sticky="nsew", padx=5, pady=5)
        self.middle_frame = middle_frame

        right_frame = CTkFrame(
            parent_frame, 
            fg_color="#343638",
            corner_radius=10,
            width=right_width, 
            height=parent_frame_height
        )
        right_frame.grid(row=0, column=2, sticky="nsew", padx=5, pady=5)
        # self.right_frame = right_frame
        right_top_frame = CTkFrame(
            right_frame,
            height=1,
            width = right_width,
            fg_color='transparent'
        )
        right_top_frame.grid(row=0, column=0, sticky='nsew')

        right_bottom_grame = CTkFrame(
            right_frame,
            width=right_width,
            fg_color='transparent'
        )
        right_bottom_grame.grid(row=1, column=0, sticky='nsew')
        self.right_frame = right_bottom_grame
        right_frame.grid_rowconfigure(1, weight=1)
        right_frame.grid_columnconfigure(0, weight=1)
        self.right_top_frame = right_top_frame

        self.display_general_info(left_frame)
        self.display_restrictions(middle_frame)
        self.display_result(self.right_frame)

    def display_general_info(self, frame: CTkFrame):
        header_label = CTkLabel(
            frame,
            text="Timestamp",
            font=("Roboto", 14, "bold"),
            text_color="#00BFFF"
        )
        header_label.pack(pady=(10, 5))

        time_label = CTkLabel(
            frame,
            text='\n'.join(self.proof.timestamp.strftime('%Y-%m-%d %H:%M').split()),
            font=("Helvetica", 16, "bold"),
            text_color="#FFFFFF",
            bg_color="transparent"
        )
        time_label.pack(expand=True, fill="both", padx=10, pady=(0, 10))

    def display_restrictions(self, frame: CTkFrame):
        header_label = CTkLabel(
            frame,
            text="Restrictions",
            font=("Roboto", 14, "bold"),
            text_color="#00BFFF"
        )
        header_label.pack(pady=(10, 5))

        for idx, restr in enumerate(self.proof.restrictions):
            self.display_restriction(frame, restr, idx+1)
    
    def display_restriction(self, frame: CTkScrollableFrame, restriction: Restriction, row_idx: int):
        restr_frame = CTkFrame(frame, bg_color='transparent', fg_color='white', height=20, corner_radius=5)
        restr_frame.pack(fill='both', expand=True, pady=5)

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
        
        functions = restriction.functions
        for key, val in [(k, functions[k]) for k in functions]:
            func_frame = CTkFrame(
                restr_frame,
                bg_color='transparent',
                fg_color='transparent'
            )
            func_frame.grid(row=row_idx, column=0, sticky='w')

            key_label = CTkLabel(
                func_frame,
                text=f'{key}:',
                font=('Roboto', 14, 'bold'),
                bg_color='transparent',
                text_color='black'
            )
            key_label.grid(row=0, column=0, padx=5, pady=0, sticky='w')

            x = symbols('x')
            terms = []
            for n, d, p in val:
                terms.append(Rational(n, d) * x**p)
            
            poly = Add(*terms)
            latex_expr = latex(sympify(poly))
            
            fig = plt.Figure(figsize=(2, 0.5), dpi=100)
            ax = fig.add_subplot(111)
            ax.set_position([0, 0, 1, 1])
            fig.subplots_adjust(left=0, right=1, top=1, bottom=0)
            ax.text(
                0, 0.5,
                f"${latex_expr}$",
                ha='left',
                va='center',
                transform=ax.transAxes,
                fontsize=12
            )
            ax.axis('off')
            canvas = FigureCanvasTkAgg(fig, master=func_frame)
            canvas_widget = canvas.get_tk_widget()
            canvas_widget.grid(row=0, column=1, padx=(0, 0), pady=0, sticky='w')
            canvas_widget.configure(height=30, width=150)
            canvas.draw()
            func_frame.grid(row=idx, column=0, sticky='w')
            idx += 1

    def display_result(self, frame: CTkFrame):
        header_label = CTkLabel(
            frame,
            text="Result",
            font=("Roboto", 14, "bold"),
            text_color="#00BFFF"
        )
        header_label.pack(pady=(10, 5))
        self.right_frame_to_destroy.append(header_label)


        result_content = CTkLabel(
            frame,
            font=("Helvetica", 16, "bold"),
            justify='center'
            )
        self.result_label = result_content
        result_content.pack(expand=True, fill="both", padx=10, pady=(0, 10))
        self.redisplay_result()

    
    def redisplay_result(self):

        is_success = self.proof.result.lower() == 'success'
        is_failure = self.proof.result.lower() == 'failure'

        text = 'Success' if is_success else 'Failure' if is_failure else 'Pending'
        text_color="#32CD32" if is_success else "#FF4500" if is_failure else 'gray'

        self.result_label.configure(text=text)
        self.result_label.configure(text_color=text_color)
        def display_proof_window():
            new_window = ctk.CTkToplevel()
            proof_text_frame = ProofTextFrame(new_window, self.proof)
            proof_text_frame.pack(expand=True, fill='both', padx=5, pady=5)
            new_window.title('New window')
            new_window.geometry('300x200')
            

        if is_success:
            proof_button = CTkButton(self.result_label.master, text='View proof', width=self.right_width-10, command=display_proof_window)
            proof_button.pack(padx=0, pady=5, expand=False)
            self.right_frame_to_destroy.append(proof_button)

        if is_success or is_failure:
            size = 15
            def suicide():
                self.destroy_proof_frame()
            button = CTkButton(
                        master=self.right_top_frame,
                        text="✕",
                        width=size,
                        height=size,
                        font=("Roboto", max(size, size // 2), "bold"),
                        text_color="white",
                        fg_color="#e94560",
                        hover_color="#ff6b81",
                        corner_radius=2,
                        command=suicide
                    )
            button.pack(side='right', padx=10, pady=5)

    def destroy_proof_frame(self):
        self.proof.destroy_json_file()
        self.destroy()
