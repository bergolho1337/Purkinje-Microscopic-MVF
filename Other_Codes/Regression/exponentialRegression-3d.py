# --------------------------------------------------------------------------------
# Program that does a 3-d regression on a set of data points given a function. 
# --------------------------------------------------------------------------------

from pylab import *
import scipy
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from mpl_toolkits import mplot3d

def fn (x, a, b, c):
    #return exp(a*x[0] + b*x[1]) + c  
    return a + b*x[0] + c*x[1]

def fn2 (x,y,a,b,c):
    #return exp(a*x + b*y) + c
    return a + b*x + c*y

data = np.genfromtxt(open("dataSegment.dat","r"))

# Define the domain xy as a 2d-array
x = scipy.array([data[:,0],data[:,1]])
y = scipy.array(data[:,2])

# Make use of the Scipy library to fit the best curve given a function 'fn' and a set of points
popt, pcov = scipy.optimize.curve_fit(fn, x, y)

# Print out the coeficients of the function
print popt

# Plot the data using Matplotlib
mx = np.linspace(min(x[0]),max(x[0]),50)
my = np.linspace(min(x[1]),max(x[1]),50)
X, Y = np.meshgrid(mx,my)
Z = fn2(X,Y,popt[0],popt[1],popt[2])

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.scatter(x[0], x[1], y, cmap='binary', linewidth=0.5)
ax.plot_surface(X,Y,Z,cmap='copper')
ax.set_title('surface')
ax.set_xlabel('alpha')
ax.set_ylabel('d1')
ax.set_zlabel('Delay');
plt.show()
