from customtkinter import CTkFrame, CTkLabel, CTkScrollableFrame, CTkEntry, CTkComboBox, CTkCanvas
import numpy as np
from config import *
import math
import itertools
from models.graph import Graph

class GraphInputView(CTkScrollableFrame):
    def __init__(self, master, save_path: str):
        super().__init__(master, orientation='vertical')
        self.num_vertices = 5
        self.max_vertices = 15
        self.save_path = save_path
        self.init_input_view()
        self.create_canvas()
        self.display_input_view()
        self.save_graph()

    def init_input_view(self):
        self.input_frame = CTkFrame(self)
        self.input_frame.pack(pady=10, padx=20, fill="x")

        self.label_num_vertices = CTkLabel(self.input_frame, text="Number of vertices:")
        self.label_num_vertices.pack(side="left", padx=(0, 10))
        self.entry_num_vertices = CTkEntry(self.input_frame, width=50)
        self.entry_num_vertices.insert(0, self.num_vertices)
        self.entry_num_vertices.bind("<Return>", self.on_change_num_vertices)
        self.entry_num_vertices.pack(side="left")

        self.label_max_vertices = CTkLabel(self.input_frame, text='Max vertices:')
        self.label_max_vertices.pack(side='left', padx=10)
        self.entry_max_vertices = CTkEntry(self.input_frame, width=50)
        self.entry_max_vertices.insert(0, self.max_vertices)
        self.entry_max_vertices.bind('<Return>', self.on_change_max_vertices)
        self.entry_max_vertices.pack(side='left')
                

        self.adj_matrix = np.full((MAX_NUM_VERTICES, MAX_NUM_VERTICES), UNKNOWN_SYMBOL)

        self.combo_frames = []
        self.combo_boxes = []

    def create_canvas(self):
        self.canvas = CTkCanvas(self, width=400, height=300)
        self.canvas.pack(pady=10, padx=20, fill="both", expand=True)

    def save_graph(self):
        graph = Graph(self.num_vertices, 
                      self.max_vertices, 
                      self.adj_matrix)
        graph.write_to_file(self.save_path)

    def on_change_num_vertices(self, event):
        n = int(self.entry_num_vertices.get())
        n = max(1, min(n, MAX_NUM_VERTICES))
        if n != self.num_vertices:
            self.num_vertices = n
            self.display_input_view()
        self.save_graph()
    
    def on_change_max_vertices(self, event):
        n = int(self.entry_max_vertices.get())
        n = max(1, n)
        self.max_vertices = n
        self.save_graph()

    def display_input_view(self):
        for frame in self.combo_frames:
            frame.destroy()
        self.combo_frames.clear()

        options = [
            CONNECTED_SYMBOL,
            NOT_CONNECTED_SYMBOL,
            UNKNOWN_SYMBOL
        ]

        self.combo_boxes = []
        label_row = CTkFrame(self)
        label_row.pack(padx=20, fill='both', expand=True)
        temp_label = CTkLabel(label_row, text=' ')
        temp_label.grid(row=0, column=0, padx=20)
        self.combo_frames.append(label_row)
        for i in range(self.num_vertices):
            column_label = CTkLabel(label_row, text=f'{i}')
            column_label.grid(row=0, column=i+1, padx=19)
        for i in range(self.num_vertices):
            row_frame = CTkFrame(self)
            row_frame.pack(padx=20, fill='both', expand=True)
            self.combo_frames.append(row_frame)
            self.combo_boxes.append([])

            row_label = CTkLabel(row_frame, text=f'{i}')
            row_label.grid(row=0, column=0, padx=10)
            for j in range(self.num_vertices):
                i_cp, j_cp = i, j
                combo_box = CTkComboBox(row_frame, width=30, 
                                        values=options, 
                                        command=lambda value, i=i_cp, j=j_cp: self.change_edge_state(i, j, value))
                # combo_box.set(self.adj_matrix[i, j])
                combo_box.grid(row=0, column=j+1)
                self.combo_boxes[i].append(combo_box)
            
        for i, j in itertools.product(range(self.num_vertices), range(self.num_vertices)):
            # self.change_edge_state(i, j, self.adj_matrix[i, j])
            self.display_combo_box_colors(i, j)

        self.draw_graph()
    
    def change_edge_state(self, i, j, value):
        self.adj_matrix[i, j] = value
        self.adj_matrix[j, i] = value
        self.display_combo_box_colors(i, j)
        self.draw_graph()
        self.save_graph()
    
    def display_combo_box_colors(self, i, j):
        value = self.adj_matrix[i, j]
        self.combo_boxes[i][j].set(value)
        self.combo_boxes[j][i].set(value)
        text_color = UNKNOWN_COLOR
        if value == CONNECTED_SYMBOL:
            text_color = CONNECTED_COLOR
        if value == NOT_CONNECTED_SYMBOL:
            text_color = NOT_CONNECTED_COLOR
        self.combo_boxes[i][j].configure(text_color=text_color)
        self.combo_boxes[j][i].configure(text_color=text_color)


    def draw_graph(self):

        self.canvas.delete("all")

        n = self.num_vertices
        x_center, y_center = 150, 150
        radius = 80
        text_offset_scalar = 1.2
        r = 5
        line_width = 2
        connected_color = CONNECTED_COLOR
        not_connected_color = NOT_CONNECTED_COLOR

        vert_pos = {}
        for i in range(n):
            alpha = 2 * math.pi / n * i
            x_vert = x_center + radius * math.cos(alpha)
            y_vert = y_center + radius * math.sin(alpha)
            vert_pos[i] = (x_vert, y_vert)
        
        for i, j in itertools.product(range(n), range(n)):
            if i > j:
                continue
            x_i, y_i = vert_pos[i]
            x_j, y_j = vert_pos[j]
            if self.adj_matrix[i, j] == CONNECTED_SYMBOL:
                self.canvas.create_line(x_i, y_i, x_j, y_j, 
                                        width=line_width, 
                                        fill=connected_color)
            if self.adj_matrix[i, j] == NOT_CONNECTED_SYMBOL:
                self.canvas.create_line(x_i, y_i, x_j, y_j, 
                                        width=line_width, 
                                        fill=not_connected_color)


        for vert in vert_pos.keys():
            x_pos, y_pos = vert_pos[vert]
            self.canvas.create_oval(x_pos - r, y_pos - r, x_pos + r, y_pos + r, fill='black')
            x_text = x_center + text_offset_scalar * (x_pos - x_center)
            y_text = y_center + text_offset_scalar * (y_pos - y_center)
            self.canvas.create_text(x_text, y_text, text=str(vert), fill='black')

        '''
        self.canvas.delete("all")

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
        '''
        