import os
import sys
import re
from matplotlib import pyplot
from pylab import genfromtxt 

names=["alien=100um","dog=165um","pig=68um","orc=200um"]

mat0 = genfromtxt("vall.txt")
pyplot.grid()
pyplot.title("Propagation Velocity -- alpha=1.0 (PMJ)")
pyplot.xlabel("Point")
pyplot.ylabel("velocity (cm/s)")
pyplot.ylim([0,300])

    
for i in range(1,5):
    pyplot.plot(mat0[:,0], mat0[:,i], label=names[i-1],marker='o')
    
pyplot.legend(loc=4)
pyplot.show()
