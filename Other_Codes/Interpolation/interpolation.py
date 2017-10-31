import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

# Delay of the bifurcation
NOBLE_BIFF_DELAY = 0.3
LIRUDY_BIFF_DELAY = 0.15

# Fazer a melhor reta que ajusta os ponto
def bestFit (X,Y):
    xbar = sum(X)/len(X)
    ybar = sum(Y)/len(Y)
    n = len(X) # or len(Y)

    numer = sum([xi*yi for xi,yi in zip(X, Y)]) - n * xbar * ybar
    denum = sum([xi**2 for xi in X]) - n * xbar**2

    b = numer / denum
    a = ybar - b * xbar

    print('best fit line:\ny = {:.2f} + {:.2f}x'.format(a, b))

    return a, b

def predActivation (f):
    global NOBLE_BIFF_DELAY
    file = open("biff2cm-alien.pth","r")
    for line in file:
        token = line.split()
        L = float(token[0])
        numBiff = float(token[1])
        print("Activation time = %.5f ms" % (f(L) + numBiff * NOBLE_BIFF_DELAY))

def readPoints ():
    x = []
    y = []
    file = open("b-coef.dat","r")
    for line in file:
        token = line.split()
        x.append(float(token[0]))
        y.append(float(token[1]))
    return x, y    

def main():
    # Read the activation times from the cable 
    x, y = readPoints()
    
    # Interpolation function
    #a, b = bestFit(x,y)
    #f = interp1d(x,y)
    f = interp1d(x,y,kind='cubic')

    # Predict the activation time using the '.pth' file
    #predActivation(f)

    # Plot the interpolation function
    xnew = np.linspace(min(x),max(x),num=41,endpoint=True)
    #ynew = [a + b * xi for xi in xnew]
    ynew = f(xnew)
    plt.plot(x,y,'o',xnew,ynew,'-')
    plt.legend(['data','bestFit'], loc='best')
    plt.xlabel("Size of the fiber (cm)")
    plt.ylabel("Activation time (ms)")
    plt.grid()
    plt.savefig("interp.pdf")

if __name__ == "__main__":
    main()

