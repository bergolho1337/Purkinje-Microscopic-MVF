#!/usr/bin/python
# -*- coding: UTF-8 -*-
# ------------------------------------------------------------------------------------------------------------------------------------
# Program that compares the analitical solution of the propagation velocity with an aproximation solution from a model
# Compares SIGMA x VELOCITY
# The analitical solution is calculated using the calibrated parameter (c)
# The output is a plot of both solutions	
# ------------------------------------------------------------------------------------------------------------------------------------

import matplotlib.pyplot as plt
import numpy as np

# (mm)
def lamb (Rm,SIGMA,d):
	return np.sqrt( (Rm * d * SIGMA * 1.0e-04) / (4.0) ) * 10

# (ms)
def tau (Rm,Cm):
	return Rm * Cm * 1.0e-03

def coef (l,t,s):
	return (s * t) / (l)

def vel (c,l,t):
	return (c * l) / t

def analiticalPlot ():
	Rm = 7.0 * 1.0e+03
	#Rc = 150.0
	D = 55
	Cm = 1.2
	c = 10.80800
	
	SIGMA = np.linspace(0.0001,0.004,100)
	v = vel(c,lamb(Rm,SIGMA,D),tau(Rm,Cm))
	plt.plot(SIGMA,v,label="Analit",color="darkblue",linewidth=2.0)

def aproximationPlot ():
    data = np.genfromtxt(open("sigma-v-Noble.dat","r"))
    plt.plot(data[:,0],data[:,1],"--",marker='o',markersize=10,label="Noble",color="darkgreen",linewidth=2.0)

def aproximationPlot2 ():
    	Rm = 7.0 * 1.0e+03
	#Rc = 150.0
	D = 120
	Cm = 1.2
	c = 10.80800
	
	SIGMA = np.linspace(0.0001,0.004,100)
	v = vel(c,lamb(Rm,SIGMA,D),tau(Rm,Cm))
	print SIGMA[48],v[48]
	plt.plot(SIGMA,v,"--",label="Li e Rudy",color="darkred",linewidth=2.0)

def main ():
	analiticalPlot()
	aproximationPlot()
	aproximationPlot2()
	#plt.title(u'Velocidade de Propagação - Analítico x Numérico')
	plt.title(u'Propagation Velocity - Analitical x Numeric')
	plt.ylabel('v (m/s)',size=15)
	plt.xlabel(r"$\sigma$ (mS/cm)",size=15)
	plt.grid()
	plt.legend(loc=0,fontsize=15)
	plt.savefig("comparison-revision.pdf")
	print("[+] Figure save at 'comparison.pdf'")

if __name__ == "__main__":
	main()
