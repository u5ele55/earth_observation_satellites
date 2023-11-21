
import matplotlib.pyplot as plt
import numpy as np

class TrajectoryDrawer:
    def __init__(self, trajectoryFilename, b, a):
        self.fileTrajectory = open(trajectoryFilename, 'r')
        self.drawTrasse = False
        self.drawZone = False
        self.withECI = False
        self.a = a # MAJOR AXIS
        self.b = b # MINOR AXIS

    def prepareTrasse(self, spotFile):
        self.drawTrasse = True
        self.spot = [float(a) for a in open(spotFile, 'r').read().split()]
    
    def prepareZone(self, zoneFile):
        self.zoneVisibility = {}
        self.drawZone = True
        file = open(zoneFile, 'r')
        N = int(file.readline())
        for i in range(N):
            self.zoneVisibility[i] = {'x': [], 'y': [], 'z': []}
            n = int(file.readline())
            for j in range(n):
                line = file.readline()
                x,y,z = [float(a) for a in line.split()]
                self.zoneVisibility[i]['x'].append(x)
                self.zoneVisibility[i]['y'].append(y)
                self.zoneVisibility[i]['z'].append(z)
        

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
            x,y,z,o = [float(a) for a in c.split()]
            mode = 'eci' if eci else 'ecef'
            
            trajectories.setdefault(id, {})
            trajectories[id].setdefault(mode, {'x': [], 'y': [], 'z': [], 'visible': []})
            
            trajectories[id][mode]['x'].append(x)
            trajectories[id][mode]['y'].append(y)
            trajectories[id][mode]['z'].append(z)
            trajectories[id][mode]['visible'].append(int(o))
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
            ax.scatter(data['x'][0], data['y'][0], data['z'][0], label='Start', c='#FFFF00')
        # trasse
        fig = plt.figure()
        ax = fig.add_subplot()
        if self.drawTrasse:
            self.__drawTrasse(trajectories, ax)
            if self.drawZone:
                self.__drawZone(ax) 
        

    def __drawTrasse(self, trajectories, ax):
        img = plt.imread("../visualize/map.jpg")
        plt.imshow(img, extent=[-180, 180, -90, 90])
        ax.set_aspect("auto")

        for id in trajectories:
            xd = np.array(trajectories[id]['ecef']['x'])
            yd = np.array(trajectories[id]['ecef']['y'])
            zd = np.array(trajectories[id]['ecef']['z'])
            visible = np.array(trajectories[id]['ecef']['visible'])

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
            
            hex = list('0123456789ABCDEF')
            diff = 7
            not_vis_c_idx = np.random.choice(16 - diff, 6)
            vis_c_idx = [idx + diff for idx in not_vis_c_idx]

            not_vis_c = '#' + ''.join([hex[idx] for idx in not_vis_c_idx])
            vis_c = '#' + ''.join([hex[idx] for idx in vis_c_idx])
            
            ax.scatter(lmbd, phi, c=[vis_c if v else not_vis_c for v in visible], s=1)
            ax.scatter(lmbd[0], phi[0], label=f'start {id}', c=vis_c)
            ax.scatter(lmbd[-1], phi[-1], label=f'end {id}', c=not_vis_c)

        # set spot
        ax.scatter(self.spot[1], self.spot[0], label='spot', c="#00FFFF")
        # ax.legend()
    
    def __drawZone(self, ax):
        for i in self.zoneVisibility:
            xd = np.array(self.zoneVisibility[i]['x'])
            yd = np.array(self.zoneVisibility[i]['y'])
            zd = np.array(self.zoneVisibility[i]['z'])
            r1 = np.sqrt(xd**2 + yd**2)
            alpha = 1/298.257
            phi = np.arcsin(zd / np.sqrt((1-alpha)**2 * r1**2 + zd**2))
            lmbd = np.arctan2(yd,xd)

            for i in range(len(lmbd)-1): 
                if abs(lmbd[i] - lmbd[i+1]) > np.pi:
                    lmbd[i] = np.nan

            lmbd *= 180 / np.pi
            phi *= 180 / np.pi
            plt.fill(lmbd, phi, edgecolor='r', fill=False)

        
