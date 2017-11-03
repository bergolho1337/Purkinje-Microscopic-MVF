# --------------------------------------------------------------------------------
# Program that does a n-dimensional regression on a set of data points given a function. 
# --------------------------------------------------------------------------------

from pylab import *
import scipy
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from mpl_toolkits import mplot3d

# Mean error 0.171577 (Tseg)
def func0 (x,y,w,z):
    coef = [3.81768672,-0.01402317,11.99991418,-0.02819055,2.7752069] 
    return coef[0]*x + coef[1]*y + coef[2]*w + coef[3]*z + coef[4]

# Mean error 13.83655 (Tdelay)
def func1 (x,y,w,z):
    coef = [-1.89203448e-01,9.64328929e-03,-2.06783057e+04,6.94608136e+01,5.06061084e+01] 
    return exp(coef[0]*x) + coef[1]*y + coef[2]*w + coef[3]*z + coef[4] 

# Mean error 15.836776 (Tdelay)
def func2 (x,y,w,z):
    coef = [-5.67876920e-01,9.64330005e-03,-2.05569596e+04,6.94608139e+01,-7.93294263e+00]
    return coef[0]*x + coef[1]*y + exp(coef[2]*w) + coef[3]*z + coef[4]

# Mean error 16.254882 (Tdelay)
def func3 (x,y,w,z):
    coef = [-5.67876936e-01,9.64329312e-03,-2.06783055e+04,-1.51065965e+02,8.19586557e+01]
    return coef[0]*x + coef[1]*y + coef[2]*w + exp(coef[3]*z) + coef[4]

# Mean error 13.985705 (Tdelay)
def func4 (x,y,w,z):
    coef = [7.72908539e-05,9.64513155e-03,-2.06783095e+04,6.94608202e+01,7.39484696e+03,-7.34074613e+03] 
    return coef[5]*exp(coef[0]*x) + coef[1]*y + coef[2]*w + coef[3]*z + coef[4]

# Mean error 13.837032 (Tdelay)
def func5 (x,y,w,z):
    coef = [1.00001212e+00,9.64325686e-03,-2.06783057e+04,6.94608137e+01,5.10851350e+01,1.06585049e+03,-1.06485067e+03] 
    return coef[5]*exp(coef[0]*x + coef[6]) + coef[1]*y + coef[2]*w + coef[3]*z + coef[4]

# Mean error 13.815030 (Ttotal)
def func6 (x,y,w,z):
    return func0(x,y,w,z) + func1(x,y,w,z) 

# ------------------------------------------------------
# Test function of the regression
def fn (x, a, b, c, d, e):
    return a*x[0] + b*x[1] + c*x[2] + d*x[3] + e
# ------------------------------------------------------

# Calculate the mean error of the regression
def calcMeanError (data):
    error = np.zeros(len(data))
    for i in range(len(data)):
        error[i] = abs(data[i,4]-func6(data[i,0],data[i,1],data[i,2],data[i,3]))
    mean_error = scipy.mean(error)
    print("Mean error of the regression = %.6f ms" % mean_error)

data = np.genfromtxt(open("dataTotal.dat","r"))

# Define the domain xy as a 2d-array
x = scipy.array([data[:,0],data[:,1],data[:,2],data[:,3]])
y = scipy.array(data[:,4])

# Make use of the Scipy library to fit the best curve given a function 'fn' and a set of points
popt, pcov = scipy.optimize.curve_fit(fn, x, y)

# Print out the coeficients of the function
print popt

# Calculate the mean error of the regression
calcMeanError(data)

#print("Eval = %.6f\n" % func6(2.00000000,200.00000000,0.00300000,0.68750000))

# ------------------------------------------------------------------------------------------
# TDelay
# coef = [-1.89203448e-01,9.64328929e-03,-2.06783057e+04,6.94608136e+01,5.06061084e+01]
# Tdelay = exp(coef[0]*x) + coef[1]*y + coef[2]*w + coef[3]*z + coef[4]

# TSeg
# coef = [3.81768672,-0.01402317,11.99991418,-0.02819055,2.7752069] 
# Tseg = coef[0]*x + coef[1]*y + coef[2]*w + coef[3]*z + coef[4]
# ------------------------------------------------------------------------------------------
