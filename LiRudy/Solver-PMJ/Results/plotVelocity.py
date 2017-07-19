import os
import sys
import re
from matplotlib import pyplot
from matplotlib.backends.backend_pdf import PdfPages
from pylab import genfromtxt 

input_filename = sys.argv[1]
output_filename = input_filename.split(".")[0]+str(".pdf")
names=["alien=100um","dog=165um","orc=200um","pig=68um"]
pp = PdfPages(output_filename)

mat0 = genfromtxt(input_filename)
pyplot.grid()
pyplot.title("Propagation Velocity -- " + input_filename + " (PMJ)")
pyplot.xlabel("Point")
pyplot.ylabel("velocity (cm/s)")
pyplot.ylim([0,300])
    
for i in range(1,5):
    pyplot.plot(mat0[:,0], mat0[:,i], label=names[i-1],marker='o')
    
pyplot.legend(loc=3)
pp.savefig()
pp.close()
#pyplot.savefig()
#pyplot.show()
