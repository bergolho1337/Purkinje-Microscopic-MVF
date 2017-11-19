# Codigo para montar a figura do delay
# Passar os arquivos .dat contendo o potencial transmebranico dos volumes de Purkinje e o do miocardio

import os
import sys
import re
from matplotlib import pyplot
from pylab import genfromtxt

def plotDelay ():
	pk_filename = "data75-LiRudy.dat"		
	mio_filename = "data100-LiRudy.dat"
	mat0 = genfromtxt(pk_filename)
	mat1 = genfromtxt(mio_filename)
	pyplot.plot(mat0[:,0], mat0[:,1], label = "Purkinje", linestyle ="-", linewidth=2)
	pyplot.plot(mat1[:,0], mat1[:,1], label = "Miocardium", linestyle ="--", linewidth=2)
	pyplot.grid()
	pyplot.title("Propagation velocity x Level")
	pyplot.xlabel("Level of the tree")
	pyplot.ylabel("Velocity (cm/s)")
	pyplot.legend()
	pyplot.savefig("delay-LiRudy.pdf")

def plotAP ():
	filename = "noble59.dat"
	data = genfromtxt(filename)
	pyplot.plot(data[:,0], data[:,1], label = "Vm", linestyle ="-", color="k", linewidth=2)
	pyplot.tick_params(
				    axis='x',          # changes apply to the x-axis
				    which='both',      # both major and minor ticks are affected
				    bottom='off',      # ticks along the bottom edge are off
				    top='off',         # ticks along the top edge are off
				    labelbottom='off') # labels along the bottom edge are off
	pyplot.yticks(size=18)
	pyplot.xlim([400,900])
	pyplot.ylim([-90,50])
	pyplot.title("Potencial de Acao")
	#pyplot.xlabel("Tempo (ms)")
	pyplot.ylabel("Potencial Transmembranico (mV)", size=15)
	pyplot.legend()
	pyplot.savefig("AP.png")

def main ():    	
	plotDelay()
	plotPA()

if __name__ == "__main__":
    main()
