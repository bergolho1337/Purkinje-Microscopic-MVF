import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

# Read points from file
x = []
y = []
file = open("points.dat","r")
for line in file:
    token = line.split()
    x.append(float(token[0]))
    y.append(float(token[1]))

# Interpolation function
f = interp1d(x,y)
#f2 = interp1d(x,y,kind='cubic')

xnew = np.linspace(min(x),max(x),num=41,endpoint=True)
plt.plot(x,y,'o',xnew,f(xnew),'-')
plt.legend(['data','linear'], loc='best')
plt.grid()
plt.show()