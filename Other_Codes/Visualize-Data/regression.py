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

def func (x1,x2,coef):
    return (coef[3] / (coef[0]*x1**3 + coef[1]*x2**3 + coef[2]))

# ------------------------------------------------------
# Test function of the regression
def fn (x, a, b, c, d):
    return (d / (a*x[0]**3 + b*x[1]**3 + c))                          

# ------------------------------------------------------
# Calculate the mean error of the regression for the Ttotal
def calcMeanError (data,coef):
    error = np.zeros(len(data))
    for i in range(len(data)):
        x1 = data[i,0]
        x2 = data[i,1]
        aprox = func(x1,x2,coef)
        analit = data[i,2]
        error[i] = abs(aprox - analit)
        print("(%f %f) = %f -- (Analit = %f) [Error = %f]" % (x1,x2,aprox,analit,error[i]))
    mean_error = scipy.mean(error)
    print("Mean error of the regression = %.6f ms" % mean_error)

def visualizeRegression (data,coef):
    x = np.linspace( min(data[:,0]) , max(data[:,0]) , 30 )
    y = np.linspace( min(data[:,1]) , max(data[:,1]) , 30 )
    X, Y = np.meshgrid(x,y)
    Z = func(X,Y,coef)
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.plot_surface(X,Y,Z)
    plt.show()

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
    x = scipy.array([data[:,0],data[:,1]])    
    y = scipy.array(data[:,2])                                              

    # Make use of the Scipy library to fit the best curve given a function 'fn' and a set of points
    popt, pcov = scipy.optimize.curve_fit(fn, x, y)

    # Print out the coeficients of the function
    print popt

    # Calculate the mean error of the regression
    calcMeanError(data,popt)

    # Visualize the surface of the regression versus the points data
    visualizeRegression(data,popt)

if __name__ == "__main__":
    main()
