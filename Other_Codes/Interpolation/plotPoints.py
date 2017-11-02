# Program that reads an input file storing (x,y,z) points and plot its data depending on the user choices
# ---------------------------------------------------------------
# Usage:> python plotPoints.py --<plot-format> <input-file>
#   <plot-format> = --scatter, --surface
#   <input-file>  = Name of the input file  
# ---------------------------------------------------------------

import sys
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

def plotScatter (data):
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.scatter(data[:,0], data[:,1], data[:,2], cmap='binary', linewidth=0.5)
    ax.set_title('scatter')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z');
    plt.show()
    #ax.view_init(0,90)
    #plt.savefig("output-xz.pdf")
    #ax.view_init(0,180)
    #plt.savefig("output-yz.pdf")

def plotSurface (data):
    X, Y = np.meshgrid(data[:,0], data[:,1])
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.plot_trisurf(data[:,0], data[:,1], data[:,2], cmap='copper', edgecolor='none')
    ax.set_title('surface')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z');
    #plt.show()
    plt.savefig("output.pdf")

def main ():
    
    if (len(sys.argv)-1 != 2):
        print("---------------------------------------------------------------")
        print("Usage:> python plotPoints.py --<plot-format> <input-file>")
        print("  <plot-format> = --scatter, --surface")
        print("  <input-file>  = Name of the input file")
        print("---------------------------------------------------------------")
        sys.exit()

    # Catch input parameters
    pformat = sys.argv[1][2:]
    filename = sys.argv[2] 
    
    # Open the file and store its data on the format (x,y,z)
    data = np.genfromtxt(open(filename,"r"))

    # Switch to type of plot
    if (pformat == "scatter"):
        plotScatter(data)
    elif (pformat == "surface"):
        plotSurface(data)



if __name__ == "__main__":
    main()
