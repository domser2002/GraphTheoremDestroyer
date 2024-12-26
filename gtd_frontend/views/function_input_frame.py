from customtkinter import CTkFrame
from customtkinter import CTkLabel
from customtkinter import CTkEntry
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import tkinter as tk
from sympy import sympify, latex, Poly, symbols

def render_polynomial(expression, canvas, fig, input_field):
    try:
        # Parse and convert input to LaTeX
        polynomial = sympify(expression)
        latex_expr = latex(polynomial)
        # Clear the figure and redraw the LaTeX expression
        fig.clear()
        ax = fig.add_subplot(111)
        ax.text(0.5, 0.5, f"${latex_expr}$", fontsize=10, ha='center', va='center')
        ax.axis('off')
        canvas.draw()
    except Exception as e:
        # Clear the figure and display an error message
        fig.clear()
        ax = fig.add_subplot(111)
        ax.text(0.5, 0.5, f"Invalid polynomial", fontsize=10, color='red', ha='center', va='center')
        ax.axis('off')
        canvas.draw()
    finally:
        # Refocus on the input field
        input_field.focus_set()

class FunctionInputFrame(CTkFrame):
    def __init__(self, master):
        super().__init__(master)
        self.expression = None

        polynomial_input = CTkEntry(self, placeholder_text="e.g., 3*x**2 + 2*x - 5")
        polynomial_input.pack(pady=5, fill='x', padx=20)

        def on_input_change(event):
            input_text = polynomial_input.get()
            self.expression = input_text
            render_polynomial(input_text, canvas, fig, polynomial_input)
        
        polynomial_input.bind("<KeyRelease>", on_input_change)

        # Create a Matplotlib figure and canvas for rendering output
        fig = plt.figure(figsize=(5, 1))  # Keeping width but setting height
        canvas = FigureCanvasTkAgg(fig, master=self)
        canvas_widget = canvas.get_tk_widget()

        canvas_widget.pack(pady=5, padx=10, fill='both', expand=True)
        
        # Set the canvas height to 30
        canvas_widget.config(height=20)
        canvas_widget.config(width=80)

    def get_value(self):
        x = symbols('x')
        try:
            poly = Poly(self.expression, x)
            terms = poly.terms()
            result = [(int(x[1]), int(x[0][0])) for x in terms]
            return result
        except:
            return None