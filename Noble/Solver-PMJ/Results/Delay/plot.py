import os
import sys
import re
from matplotlib import pyplot
from pylab import genfromtxt 

names=["orc=200um","dog=165um","alien=100um","pig=68um"]

mat0 = genfromtxt("delay.txt")
pyplot.grid()
pyplot.title("Delay (PMJ)")
pyplot.xlabel("alpha")
pyplot.ylabel("Delay (s)")
pyplot.xlim([0.5,2.0])
#pyplot.ylim([0,300])

    
for i in range(1,5):
    pyplot.plot(mat0[:,0], mat0[:,i], label=names[i-1],marker='o')
    
pyplot.legend(loc=2)
pyplot.show()
