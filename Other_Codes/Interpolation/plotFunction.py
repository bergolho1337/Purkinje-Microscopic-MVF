'''
========================
3D surface (solid color)
========================

Demonstrates a very basic plot of a 3D surface using a solid color.
'''

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Make data
x = np.arange(0,100,1)
y = np.arange(0,100,1)
x, y = np.meshgrid(x,y)
z = x + y**2

# Plot the surface
ax.plot_surface(x, y, z, color='b')

plt.show()