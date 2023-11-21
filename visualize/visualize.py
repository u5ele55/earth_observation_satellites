from trajectory import *
from geoid import GeoidDrawer
from designations import DesignationsDrawer
# from test import TestDrawer
from test2 import TestDrawer

MAJOR_AXIS = 6378137; 
MINOR_AXIS = 6356752.3142; 

if __name__ == '__main__':
    ax = plt.figure(1).add_subplot(projection='3d')
    trdr = TrajectoryDrawer(r"trajectory.txt", MINOR_AXIS, MAJOR_AXIS)
    geoidDr = GeoidDrawer(MINOR_AXIS, MAJOR_AXIS)
    testDr = TestDrawer(r"test.txt")

    geoidDr.draw(ax)
    #testDr.draw(ax)
    trdr.prepareTrasse(r'../spot.txt')
    trdr.prepareZone(r"zone.txt")
    inECEF = False
    trdr.draw(ax, inECEF, r"telescopes.txt")

    plt.show()