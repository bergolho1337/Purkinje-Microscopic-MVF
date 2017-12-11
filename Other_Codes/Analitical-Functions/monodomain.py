import matplotlib.pyplot as plt
import numpy as np
import math

# (mm)
def lamb (sigma,d,Rm,L):
	return ( np.sqrt( (Rm * sigma * per(d)) / (beta(d) * L)) ) * 10.0

# (ms)
def tau (Cm,Rm,L):
	return Cm * Rm

# (cm)
def per (d):
    return math.pi * d

# (cm^2)
def area (d):
    return math.pi * d * d / 4.0

# (cm^3)
def volume (d,L):
    return area(d) * L

# (cm^-1)
def beta (d):
    return 4.0 / d

def coef (l,t,s):
	return (s * t) / (l)

def vel (c,l,t):
	return (c * l) / t

def velocity_Monodomain ():
    sigma = 0.004
    d = 35.0 * 1.0e-04
    L = 164.0 * 1.0e-04
    Rm = 7.0 * 1.0e+03
    Cm = 1.2
    s = 2.5
    data = np.genfromtxt(open("d1_x_v.dat","r"))

    l = lamb(sigma,d,Rm,L)
    t = tau(Cm,Rm,L)
    c = coef(l,t,s)

    D = np.linspace(0.001,0.005,500)
    SIGMA = np.linspace(0.0001,0.0008,500)
    v = vel(c,lamb(sigma,D,Rm,L),tau(Cm,Rm,L))

    plt.plot(D,v)
    plt.plot(data[:,0],data[:,1])

    plt.title('Monodomain Equation - Mammalian Cardiac Cell')
    plt.ylabel('v (m/s)')
    plt.xlabel('d (cm)')
    plt.grid()
    plt.savefig("monodomain_d.pdf")

    v = vel(c,lamb(SIGMA,d,Rm,L),tau(Cm,Rm,L))
    plt.clf()
    plt.plot(SIGMA,v)
    plt.title('Monodomain Equation - Mammalian Cardiac Cell')
    plt.ylabel('v (m/s)')
    plt.xlabel('sigma (mS/cm)')
    plt.grid()
    plt.savefig("monodomain_sigma.pdf")

def main ():
    velocity_Monodomain()

	
if __name__ == "__main__":
	main()
