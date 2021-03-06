#!/usr/bin/python
# -*- coding: UTF-8 -*-
# Codigo para montar a figura do delay
# Passar os arquivos .dat contendo o potencial transmebranico dos volumes de Purkinje e o do miocardio

import os
import sys
import re
from matplotlib import pyplot
from pylab import genfromtxt

def plotDelay ():
	pk_filename = "data232-LiRudy-2.dat"		
	mio_filename = "data242-LiRudy-2.dat"
	mat0 = genfromtxt(pk_filename)
	mat1 = genfromtxt(mio_filename)
	pyplot.plot(mat0[:,0], mat0[:,1], label = "Purkinje", linestyle ="-", linewidth=2, color="black")
	pyplot.plot(mat1[:,0], mat1[:,1], label = u"Miocárdio", linestyle ="--", linewidth=2, color="black")
	pyplot.grid()
	pyplot.tick_params(labelsize=12)
	pyplot.xlim([0,50])
	pyplot.title(u"Tempo ativação - Purkinje x Miocárdio (LiRudy)",size=15)
	pyplot.xlabel(u"Tempo de ativação (ms)", size=15)
	pyplot.ylabel(u"Potencial transmembrânico (mV)", size=15)
	pyplot.legend(loc=0,prop={'size': 16})
	pyplot.savefig("delay-LiRudy-block.pdf")
	#pyplot.savefig("delay-Noble.pdf")
	#pyplot.savefig("delay-Noble.eps",format="eps",dpi=1000)

def plotAP ():
	filename = "noble-2000ms.dat"
	data = genfromtxt(filename)
	pyplot.plot(data[:,0], data[:,1], label = "Vm", linestyle ="-", color="k", linewidth=2)
	#pyplot.tick_params(
	#			    axis='x',          # changes apply to the x-axis
	#			    which='both',      # both major and minor ticks are affected
	#			    bottom='off',      # ticks along the bottom edge are off
	#			    top='off',         # ticks along the top edge are off
	#			    labelbottom='off') # labels along the bottom edge are off
	pyplot.yticks(size=15)
	pyplot.xlim([0,2000])
	pyplot.ylim([-90,50])
	pyplot.title("Potencial de Acao - Noble (1962)",size=18)
	pyplot.xlabel("Tempo (ms)", size=18)
	pyplot.ylabel("Potencial Transmembranico (mV)", size=18)
	pyplot.legend()
	pyplot.savefig("AP_2.pdf")

def main ():    	
	plotDelay()
	#plotAP()

if __name__ == "__main__":
    main()
