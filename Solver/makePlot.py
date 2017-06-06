import sys
from matplotlib import pyplot
from pylab import genfromtxt  
 
if (len(sys.argv) < 2):
    print "=============================================================================="
    print "Usage:> python ", sys.argv[0], " <data_file> <volume_id>"
    print "Example:> python makePlot.py data258.dat 258"
    print "=============================================================================="
    sys.exit()


mat0 = genfromtxt(sys.argv[1])
pyplot.plot(mat0[:,0], mat0[:,1], label = "Vm")
pyplot.grid()
pyplot.title(str("Volume = " + sys.argv[2]))
pyplot.xlabel("Time (ms)")
pyplot.ylabel("Transmembrane potential (mV)")
pyplot.legend()
#pyplot.show()
pyplot.savefig(str("potential" + sys.argv[2] + ".png"))
