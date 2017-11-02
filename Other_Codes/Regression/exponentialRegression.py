from pylab import *
import scipy
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

data = np.genfromtxt(open("dataSegment.dat","r"))

# Try to fit a exponential of the type: y = a*exp(b*t)
popt, pcov = scipy.optimize.curve_fit(lambda t,a,b: a*np.exp(b*t),data[:,0],data[:,1])
print("Without the initial guess = " + str(popt))
print("%.5f * exp(%.5f * t)" % (popt[0],popt[1]))

popt, pcov = scipy.optimize.curve_fit(lambda t,a,b: a*np.exp(b*t),data[:,0],data[:,1],p0=(5, 0.1))
print("Without the initial guess = " + str(popt))
print("%.5f * exp(%.5f * t)" % (popt[0],popt[1]))

X = np.linspace(min(data[:,0]),max(data[:,0]),50)
Y = popt[0] * np.exp(popt[1]*X)

plt.title("Exponential Data")
plt.xlabel('x')
plt.ylabel('y')
plt.plot(data[:,0],data[:,1],'bo',X,Y,'r-')
plt.grid()
plt.show()
