from pylab import *
import scipy
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from mpl_toolkits import mplot3d

def func (x,y):
    # (0.34375, 30)
    #return 1.39980 * exp(3.77273 * x) + 1531.58933 * exp(-0.19697 * y)
    # (0.171875, 20)
    return 1.39980 * exp(3.77273 * x) + 27.17048 * exp(-0.07665 * y)

data = np.genfromtxt(open("data.dat","r"))

x = np.linspace(min(data[:,0]),max(data[:,0]),20)
y = np.linspace(min(data[:,1]),max(data[:,1]),20)
X, Y = np.meshgrid(x,y)
Z = func(X,Y)

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot_surface(X,Y,Z, cmap='copper', edgecolor='none')
ax.scatter(data[:,0], data[:,1], data[:,2], cmap='binary', linewidth=0.5)
ax.set_title('surface')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z');
plt.show()
#plt.savefig("output.pdf")
