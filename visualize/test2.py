import numpy as np
import matplotlib.pyplot as plt

class TestDrawer:
    def __init__(self, file):
        self.file = open(file, 'r')

    def draw(self, ax):
        text = self.file
        for line in text:
            if line:
                x_p, y_p, z_p = [float(a) for a in line[1:-2].split()]
                ax.scatter(x_p, y_p, z_p, c="#000000")