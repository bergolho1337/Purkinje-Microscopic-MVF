import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate


x = np.arange(0, 2*np.pi+np.pi/4, np.pi/4)
y = np.cos(x)
tck = interpolate.splrep(x, y, s=0)
xnew = np.arange(0, 2*np.pi, np.pi/50)
ynew = interpolate.splev(xnew, tck, der=0)

plt.plot(x, y, 'x', xnew, ynew, xnew, np.sin(xnew), x, y, 'b')
plt.legend(['Linear', 'Cubic Spline', 'True'])
plt.axis([min(xnew), max(xnew), min(ynew)-0.1, max(ynew)+0.1])
plt.title('Cubic-spline interpolation')
plt.grid()
plt.show()