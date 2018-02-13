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
	D = [30.0,27.0,24.3,21.87]

	for d in D:
		v = vel(c,lamb(Rm,Rc,d),tau(Rm,Cm))
		print str(d) + " " + str(v)

def main ():
	analiticalPlot()

if __name__ == "__main__":
	main()
