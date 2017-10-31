from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D
import random

def readPoints (filename):
    file = open(filename,'r')
    x = []
    y = []
    z = []
    for line in file:
        token = line.split()
        x.append(float(token[0]))
        y.append(float(token[1]))
        z.append(float(token[2]))
    return x, y, z

fig = pyplot.figure()
ax = Axes3D(fig)

x,y,z = readPoints("2d-points.dat")
#x = list(range(0, 100))
#y = list(range(0, 100))
#z = list(range(0, 100))

#random.shuffle(x)
#random.shuffle(y)
#random.shuffle(z)

ax.scatter(x,y,z)
#ax.plot_surface(z,y,z)
pyplot.show()