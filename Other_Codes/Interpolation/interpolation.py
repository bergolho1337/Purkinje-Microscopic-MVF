import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

# Points to interpolate
x = np.linspace(0,10,num=11,endpoint=True)
y = np.cos(-x**2 / 9.0)

# Interpolation function
f = interp1d(x,y)
f2 = interp1d(x,y,kind='cubic')

# Plot the interpolation
xnew = np.linspace(0,10,num=41,endpoint=True)
plt.plot(x,y,'o',xnew,f(xnew),'-',xnew,f2(xnew),'--')
plt.legend(['data','linear','cubic'], loc='best')
plt.show()