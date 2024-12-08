from customtkinter import CTkFrame, CTkLabel, CTkScrollableFrame, CTkEntry, CTkComboBox, CTkCanvas
import numpy as np
from config import *

class GraphInputView(CTkScrollableFrame):
    def __init__(self, master):
        super().__init__(master, orientation='vertical')
        self.num_vertices = 5
        self.init_input_view()
        self.create_canvas()
        self.display_input_view()

    def init_input_view(self):
        self.input_frame = CTkFrame(self)
        self.input_frame.pack(pady=10, padx=20, fill="x")
        self.label_num_vertices = CTkLabel(self.input_frame, text="Number of vertices:")
        self.label_num_vertices.pack(side="left", padx=(0, 10))
        self.entry_num_vertices = CTkEntry(self.input_frame, width=50)
        self.entry_num_vertices.insert(0, self.num_vertices)
        self.entry_num_vertices.bind("<Return>", self.on_change_num_vertices)
        self.entry_num_vertices.pack(side="left")

        # Initialize adjacency matrix
        self.adj_matrix = np.full((MAX_NUM_VERTICES, MAX_NUM_VERTICES), UNKNOWN_SYMBOL)

        # Container for combo boxes
        self.combo_frames = []

    def create_canvas(self):
        # Create and pack the canvas
        self.canvas = CTkCanvas(self, width=400, height=300)
        self.canvas.pack(pady=10, padx=20, fill="both", expand=True)

    def on_change_num_vertices(self, event):
        n = int(self.entry_num_vertices.get())
        n = max(1, min(n, MAX_NUM_VERTICES))
        if n != self.num_vertices:
            self.num_vertices = n
            self.display_input_view()

    def display_input_view(self):
        # Clear previous combo boxes
        for frame in self.combo_frames:
            frame.destroy()
        self.combo_frames.clear()

        # Clear canvas (optional, if you have to draw something)
        self.canvas.delete("all")

        options = [
            CONNECTED_SYMBOL,
            NOT_CONNECTED_SYMBOL,
            UNKNOWN_SYMBOL
        ]

        # Create new frames and combo boxes
        for i in range(self.num_vertices):
            row_frame = CTkFrame(self)
            row_frame.pack(padx=20, fill='both', expand=True)
            self.combo_frames.append(row_frame)

            for j in range(self.num_vertices):
                combo_box = CTkComboBox(row_frame, width=30, values=options)
                combo_box.set(self.adj_matrix[i, j])
                combo_box.grid(row=0, column=j)

        # Here you can also add drawing logic to the canvas if needed
        self.draw_graph()  # Placeholder for a drawing function

    def draw_graph(self):

        self.canvas.create_oval(50, 50, 150, 150, fill="blue", outline="black")
        
        # Example of drawing a line
        self.canvas.create_line(50, 200, 200, 200, fill="green", width=3)

        # Example of drawing another circle
        self.canvas.create_oval(200, 50, 300, 150, fill="yellow", outline="black")
        
        # Example of drawing a line between circles
        self.canvas.create_line(150, 100, 250, 100, fill="red", width=2)

        # Example of adding text to the canvas
        self.canvas.create_text(100, 250, text="This is a text", fill="black", font=("Arial", 16))
        self.canvas.create_text(200, 250, text="Another text", fill="blue", font=("Courier", 12))
        self.canvas.create_text(300, 250, text="More text", fill="green", font=("Times New Roman", 14))
        