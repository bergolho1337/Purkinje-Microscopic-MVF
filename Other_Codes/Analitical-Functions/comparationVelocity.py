#!/usr/bin/python
# -*- coding: UTF-8 -*-
# ------------------------------------------------------------------------------------------------------------------------------------
# Program that compares the analitical solution of the propagation velocity with an aproximation solution from a model
# The analitical solution is calculated using the calibrated parameter (c)
# The output is a plot of both solutions	
# ------------------------------------------------------------------------------------------------------------------------------------

import matplotlib.pyplot as plt
import numpy as np

# (mm)
def lamb (Rm,Rc,d):
	return np.sqrt( (Rm * d * 1.0e-04) / (4.0 * Rc) ) * 10

# (ms)
def tau (Rm,Cm):
	return Rm * Cm * 1.0e-03

def coef (l,t,s):
	return (s * t) / (l)

def vel (c,l,t):
	return (c * l) / t

def analiticalPlot ():
	Rm = 7.0 * 1.0e+03
	Rc = 150.0
	Cm = 1.2
	c = 10.80800
	
	D = np.linspace(10,50,500)
	v = vel(c,lamb(Rm,Rc,D),tau(Rm,Cm))
	plt.plot(D,v,label="analit",color="darkblue",linewidth=2.0)

def aproximationPlot ():
    data = np.genfromtxt(open("d1-v_Noble.dat","r"))
    plt.plot(data[:,0],data[:,1],"--",label="aprox",color="darkgreen",linewidth=2.0)

def main ():
    analiticalPlot()
    aproximationPlot()
    plt.title(u'Velocidade Propagação - Analítico x Aproximação',size=16)
    plt.ylabel('v (m/s)')
    plt.xlabel('d (um)')
    plt.grid()
    plt.legend(loc=0,fontsize=15)
    plt.savefig("comparison.pdf")
    print("[+] Figure save at 'comparison.pdf'")

if __name__ == "__main__":
	main()
