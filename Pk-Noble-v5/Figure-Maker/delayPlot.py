import os
import sys
import re
from matplotlib import pyplot
from pylab import genfromtxt

def plotDelay ():
	pk_filename = "data150.dat"		
	mio_filename = "data200.dat"
	mat0 = genfromtxt(pk_filename)
	mat1 = genfromtxt(mio_filename)
	pyplot.plot(mat0[:,0], mat0[:,1], label = "Purkinje", linestyle ="-", linewidth=2)
	pyplot.plot(mat1[:,0], mat1[:,1], label = "Miocardium", linestyle ="--", linewidth=2)
	pyplot.grid()
	pyplot.title("Propagation velocity x Level")
	pyplot.xlabel("Level of the tree")
	pyplot.ylabel("Velocity (cm/s)")
	pyplot.legend()
	pyplot.savefig("delay.pdf")

def main ():    	
	plotDelay()

if __name__ == "__main__":
    main()