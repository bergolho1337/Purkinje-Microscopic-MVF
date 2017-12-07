# --------------------------------------------------------------------------------
# Program that does a n-dimensional regression on a set of data points given a function. 
# --------------------------------------------------------------------------------

from pylab import *
import sys
import scipy
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from mpl_toolkits import mplot3d

# Mean error 0.171510 (Tseg)
def func0 (x1,x2,x3,x4,x5,coef):
    #coef = [3.81768672,-0.01402317,1.42079815,-0.02818996,2.73394343]
    return coef[0]*x1 + coef[1]*x2 + coef[2]*np.sqrt(x3) + coef[3]*x4 + coef[4]

# Mean error 11.890324 (Tdelay)
def func1 (x1,x2,x3,x4,x5,coef):
    #coef = [1.64005959e+03,-1.91835183e+03,3.40996996e-03,1.30797791e+01] 
    return coef[0]*exp(coef[1]*x3) + coef[2]*exp(coef[3]*x4)

# Mean error (TdelayBiff)
def func2 (x1,x2,x3,x4,x5,coef):
    #coef = [-2.05812575e-03,-6.37789352e-05,1.87178472e-02,-4.36363636e-03,2.19444444e+00,3.15444444e-01,-1.26703852e+00]
    return coef[0]*x1 + coef[1]*x2**2 + coef[2]*x2 + coef[3]*x3 + coef[4]*x4 + coef[5]*x5 + coef[6]

# (Ttotal)
def func3 (x1,x2,x3,x4,x5):
    return func0(x1,x2,x3,x4) + func1(x1,x2,x3,x4) + func2(x1,x2,x3,x4,x5)

# (Tterm)
def func4 (x1,x2,x3,x4,x5):
    return func0(x1,x2,x3,x4) + func2(x1,x2,x3,x4,x5)


# ------------------------------------------------------
# Test function of the regression
def fn (x, a, b, c, d):
    #return a*x[0] + b*x[1]**2 + c*x[1] + d*x[2] + e*x[3] + f*x[4] + g  # TDelayBiff
    return a*exp(b*x[2]) + c*exp(d*x[3])                                # Tdelay
    #return a*x[0] + b*x[1] + c*np.sqrt(x[2]) + d*x[3] + e              # Tseg

# ------------------------------------------------------
# Calculate the mean error of the regression for the Ttotal
def calcMeanError (data,coef):
    error = np.zeros(len(data))
    for i in range(len(data)):
        x1 = data[i,0]
        x2 = data[i,1]
        x3 = data[i,2]
        x4 = data[i,3]
        x5 = data[i,4]
        aprox = func1(x1,x2,x3,x4,x5,coef)
        analit = data[i,5]
        error[i] = abs(aprox - analit)
        print("(%f %f %f %f %f) = %f -- (Analit = %f) [Error = %f]" % (x1,x2,x3,x4,x5,aprox,analit,error[i]))
    mean_error = scipy.mean(error)
    print("Mean error of the regression = %.6f ms" % mean_error)

# -----------------------------------------------------------------------------------------------------
# MAIN FUNCTION
def main ():
    if (len(sys.argv) != 2):
        print("=====================================================")
        print("Usage:> python %s <input_data>" % sys.argv[0])
        print("=====================================================")
        sys.exit(0)

    filename = sys.argv[1]
    data = np.genfromtxt(open(filename,"r"))

    # Define the domain xy as a 2d-array
    x = scipy.array([data[:,0],data[:,1],data[:,2],data[:,3],data[:,4]])    
    y = scipy.array(data[:,5])                                              

    # Make use of the Scipy library to fit the best curve given a function 'fn' and a set of points
    popt, pcov = scipy.optimize.curve_fit(fn, x, y)

    # Print out the coeficients of the function
    print popt

    # Calculate the mean error of the regression
    calcMeanError(data,popt)

if __name__ == "__main__":
    main()

# ------------------------------------------------------------------------------------------
# x1 = lf (length fiber)
# x2 = lc (length cell)
# x3 = d1 (depth cell)
# x4 = alpha
# x5 = #biff (number of bifurcations) 

# TDelay
# coef = [1.64005959e+03,-1.91835183e+03,3.40996996e-03,1.30797791e+01] 
# coef[0]*exp(coef[1]*x3) + coef[2]*exp(coef[3]*x4)

# TSeg
# coef = [3.81768672,-0.01402317,11.99991418,-0.02819055,2.7752069] 
# Tseg = coef[0]*x1 + coef[1]*x2 + coef[2]*np.sqrt(x3) + coef[3]*x4 + coef[4]

# TDelayBiff
# coef = [-2.05812575e-03,-6.37789352e-05,1.87178472e-02,-4.36363636e-03,2.19444444e+00,3.15444444e-01,-1.26703852e+00]
# TDelayBiff = coef[0]*x1 + coef[1]*x2**2 + coef[2]*x2 + coef[3]*x4 + coef[4]*x3 + coef[5]*x5 + coef[6]
# ------------------------------------------------------------------------------------------
