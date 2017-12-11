# ------------------------------------------------------------------------------------------------------------------------------------
# Program that calibrates the celular model parameter (c) on the propagation velocity equation
# The propagation velocity equation was taken from Keener et al, 2009 (pag. 235)
# Values of the other parameters were taken from the Typical parameter values for a variety of excitable cells table (pag. 180)
# The procedure of the calibration is:
# 	1) Consider the propagation velocity equation on a cable:
#		v = c * lambda_m / tau_m
#   Where
#		lambda_m = np.sqrt( (Rm * d) / (4.0 * Rc) )
#		tau_m = Rm * Cm
#
#   2) Take the values related to the parameters on the reference table considering a mammalian cardiac cell
#		Rm = 7 kohm . cm^2
#		Rc = 150 ohm . cm
#		Cm = 1.2 uF / cm^2
#
#	3) Fix the velocity and the diameter of the cell. We choose a v = 2.6 m/s and a d = 35 um. Which are good aproximations for a
#	Purkinje cardiac cell. (Li and Rudy, 2011)
#
#	4) Estimate the (c) parameter using the formula.
#
#	5) Now using the calculated (c) parameter plot the correspond function varying de diameter.
#	
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

def mammalianCardiac_Cable ():
	Rm = 7.0 * 1.0e+03
	Rc = 150.0
	Cm = 1.2
	d = 35.0
	s = 2.6
	l = lamb(Rm,Rc,d)
	t = tau(Rm,Cm)
	c = coef(l,t,s)
	print("----------------------------------------------")
	print("Mammalian Cardiac Cell")
	print("Lambda = %.5f" % l)
	print("Tau = %.5f" % t)
	print("Celullar coefficient = %.5f" % c)
	print("Velocity = %.5f" % vel(c,l,t))
	print("----------------------------------------------")
	
	D = np.linspace(10,50,500)
	v = vel(c,lamb(Rm,Rc,D),tau(Rm,Cm))
		
	plt.plot(D,v,label="analit")
	plt.title('Cable Equation - Mammalian Cardiac Cell')
	plt.ylabel('v (m/s)')
	plt.xlabel('d (um)')
	plt.grid()
	plt.legend(loc=0)
	plt.savefig("cable_d.pdf")
	print("[+] Figure save at 'cable_d.pdf'")

def main ():

	mammalianCardiac_Cable()

if __name__ == "__main__":
	main()
