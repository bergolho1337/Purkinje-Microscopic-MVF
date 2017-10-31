# Realiza a interpolacao da funcao T_seg

import numpy as np
from scipy import interpolate
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

def readPoints (filename):
    file = open(filename,'r')
    data = []
    for line in file:
        token = line.split()
        x = float(token[0])
        y = float(token[1])
        z = float(token[2])
        data.append([x,y,z])
    return np.asarray(data)

def makeGrid (data):
    X,Y = np.meshgrid(np.arange(100, 200, 2.0), np.arange(1.0, 20.0, 2.0))
    XX = X.flatten()
    YY = Y.flatten()
    return XX, YY

def main():
    # Read the points to interpolate
    data = readPoints("2d-points.dat")

    X, Y = makeGrid(data)


if __name__ == "__main__":
    main()


