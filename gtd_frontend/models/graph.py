import numpy as np
from config import *
import json

class Graph:
    def __init__(self, num_vertices, max_vertices, adj_matrix):
        num_vertices = int(num_vertices)
        max_vertices = int(max_vertices)
        self.num_vertices = num_vertices
        self.max_vertices = max_vertices
        self.adj_matrix = adj_matrix
    
    def write_to_file(self, path: str):
        with open(path, 'w') as file:
            adj_matrix = self.adj_matrix
            adj_matrix = adj_matrix[:self.max_vertices, :self.max_vertices]
            data = {
                'num_vertices': self.num_vertices,
                'max_vertices': self.max_vertices,
                'adj_matrix': adj_matrix.tolist()
            }
            json.dump(data, file, indent=4)