import numpy as np
from scipy import interpolate
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

def printData (x,y,z):
    nx = len(x[0])
    ny = len(y[0])
    for i in range(nx):
        for j in range(ny):
            print("%.5f %.5f %.5lf" % (xx[i,j],yy[i,j],z[i,j]))

def plotInterpolation (f):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(xnew, ynew, znew, color='b')
    plt.show()

# Make data
x = np.arange(-5.01, 5.01, 0.25)
y = np.arange(-5.01, 5.01, 0.25)
nx = len(x)
ny = len(y)
xx, yy = np.meshgrid(x,y,indexing='ij')
z = np.sin(xx**2+yy**2)
#print type(z)
printData(xx,yy,z)

# Interpolate
f = interpolate.interp2d(x, y, z, kind='cubic')

xnew = np.arange(-5.01, 5.01, 1e-1)
ynew = np.arange(-5.01, 5.01, 1e-1)
znew = f(xnew,ynew)

plt.plot(x, z[0, :], 'ro-', xnew, znew[0, :], 'b-')
plt.show()