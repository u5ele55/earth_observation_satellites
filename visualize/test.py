import numpy as np
import matplotlib.pyplot as plt

class TestDrawer:
    def __init__(self, file):
        self.file = open(file, 'r')

    def draw(self, ax):
        text = self.file.read().split('\n')
        cnt = -1
        for i in [0,1]:
            cnt += 1
            # on plane
            x_p, y_p, z_p = [float(a) for a in text[cnt][1:-1].split()]
            #ax.scatter(x_p, y_p, z_p, c=['#FF0000', '#00FF00'][i])
            cnt += 1

            # projection
            x_p, y_p, z_p = [float(a) for a in text[cnt][1:-1].split()]
            ax.scatter(x_p, y_p, z_p, c=['#FF0000', '#00FF00'][i])
            cnt += 1

            N = int(text[cnt])
            cnt += 1
            # on circle
            x_p, y_p, z_p = [float(a) for a in text[cnt][1:-1].split()]
            ax.scatter(x_p, y_p, z_p, c=['#FF00FF', '#00FFFF'][i])
            cnt += 1
            for j in range(N):
                x, y, z = [float(a) for a in text[cnt + j][1:-1].split()]
                ax.scatter(x, y, z, c=['#AA0000', '#00AA00'][i])
            cnt += N-1