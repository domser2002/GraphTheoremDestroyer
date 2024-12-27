from customtkinter import CTkFrame, CTkEntry, CTk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import tkinter as tk
from sympy import sympify, latex, Poly, symbols

def render_polynomial(expression, canvas, fig, input_field, is_destroyed_callback):
    if is_destroyed_callback():
        return

    try:
        x = symbols('x')
        poly = Poly(sympify(expression), x)
        
        if not all(coef.is_rational for coef in poly.all_coeffs()):
            raise ValueError("Not a rational polynomial")
        
        latex_expr = latex(sympify(expression))
        
        fig.clear()
        ax = fig.add_subplot(111)
        ax.text(0.5, 0.5, f"${latex_expr}$", fontsize=10, ha='center', va='center')
        ax.axis('off')
        
        canvas.draw()
        
    except Exception as e:
        fig.clear()
        ax = fig.add_subplot(111)
        ax.text(0.5, 0.5, "Invalid polynomial", fontsize=10, color='red', ha='center', va='center')
        ax.axis('off')
        canvas.draw()
    finally:
        if not is_destroyed_callback():
            input_field.focus_set()

class FunctionInputFrame(CTkFrame):
    def __init__(self, master):
        super().__init__(master)
        
        self._is_destroyed = False
        self.expression = None
        
        self.polynomial_input = CTkEntry(
            self,
            placeholder_text="e.g., 3*x**2 + 2*x - 5"
        )
        self.polynomial_input.pack(pady=5, fill='x', padx=20)
        
        self.fig = plt.Figure(figsize=(5, 1))
        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        canvas_widget = self.canvas.get_tk_widget()
        canvas_widget.pack(pady=5, padx=10, fill='both', expand=True)
        
        canvas_widget.config(height=25, width=80)
        
        self.polynomial_input.bind("<KeyRelease>", self.on_input_change)
        
        render_polynomial("", self.canvas, self.fig, self.polynomial_input, self.is_destroyed)
    
    def is_destroyed(self):
        return self._is_destroyed
    
    def on_input_change(self, event):
        if self.is_destroyed():
            return
        
        input_text = self.polynomial_input.get()
        self.expression = input_text
        render_polynomial(
            input_text,
            self.canvas,
            self.fig,
            self.polynomial_input,
            self.is_destroyed
        )
    
    def get_value(self):
        x = symbols('x')
        try:
            poly = Poly(self.expression, x)
            terms = poly.terms()
            result = [(coef.p, coef.q, deg[0]) for deg, coef in terms]
            return result
        except:
            return None
    
    def destroy(self):
        if self._is_destroyed:
            return
        
        self._is_destroyed = True
        
        self.polynomial_input.unbind("<KeyRelease>")
        
        try:
            self.canvas.get_tk_widget().destroy()
            plt.close(self.fig)
        except Exception as e:
            print(f"Error during cleanup: {e}")
        
        super().destroy()
