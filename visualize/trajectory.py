
import matplotlib.pyplot as plt
import numpy as np

class TrajectoryDrawer:
    def __init__(self, trajectoryFilename, b, a):
        self.fileTrajectory = open(trajectoryFilename, 'r')
        self.drawTrasse = False
        self.withECI = False
        self.a = a # MAJOR AXIS
        self.b = b # MINOR AXIS

    def prepareTrasse(self):
        self.drawTrasse = True

    def draw(self, ax, inECEF = False, telescopesECEFFile = None):
        data = self.fileTrajectory.read().split('\n')
        N = int(data[0])
        trajectories = {
            # id: {
            #   'eci': {
            #       'x': [], 'y': [], 'z': [],
            #   }
            # }
        }
        eci = True
        id = 0
        
        for c in data[1:-1]:
            x,y,z = [float(a) for a in c.split()]
            mode = 'eci' if eci else 'ecef'
            
            trajectories.setdefault(id, {})
            trajectories[id].setdefault(mode, {'x': [], 'y': [], 'z': []})
            
            trajectories[id][mode]['x'].append(x)
            trajectories[id][mode]['y'].append(y)
            trajectories[id][mode]['z'].append(z)
            eci = not eci
            if eci:
                id += 1
                id %= N
        # # Scaling
        # to_all = lambda f, ar1, ar2, ar3: f(f(ar1), f(ar2), f(ar3))
        # l = to_all(min, xs, ys, zs)
        # u = to_all(max, xs, ys, zs)

        # ax.set_xlim3d(l, u)
        # ax.set_ylim3d(l, u)
        # ax.set_zlim3d(l, u)
        
        # ECI trajectory
        for id in trajectories:
            data = trajectories[id]['eci']    
            ax.plot(data['x'], data['y'], data['z'], label='SC trajectory eci', c='#FF0000')
            ax.scatter(data['x'][0], data['y'][0], data['y'][0], label='Start', c='#FFFF00')

        # trasse
        if not self.drawTrasse:
            return

        fig = plt.figure()
        ax = fig.add_subplot()
        img = plt.imread("../visualize/map.jpg")
        plt.imshow(img, extent=[-180, 180, -90, 90])
        ax.set_aspect("auto")

        for id in trajectories:
            xd = np.array(trajectories[id]['ecef']['x'])
            yd = np.array(trajectories[id]['ecef']['y'])
            zd = np.array(trajectories[id]['ecef']['z'])

            r1 = np.sqrt(xd**2 + yd**2)
            alpha = 1/298.257
            phi = np.arcsin(zd / np.sqrt((1-alpha)**2 * r1**2 + zd**2))
            lmbd = np.arctan2(yd,xd)

            # discontinuities
            for i in range(len(lmbd)-1): 
                if lmbd[i] - lmbd[i+1] > np.pi:
                    lmbd[i] = np.nan

            lmbd *= 180 / np.pi
            phi *= 180 / np.pi

            ax.plot(lmbd, phi)
            ax.scatter(lmbd[0], phi[0], label=f'start {id}', c="#00FF00")
            ax.scatter(lmbd[-1], phi[-1], label=f'end {id}', c='#FF0000')

        
        ax.legend()
        
