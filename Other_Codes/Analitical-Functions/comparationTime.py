# ------------------------------------------------------------------------------------------------------------------------------------
# Program that compares the analitical solution of the activation time with an aproximation solution from a model
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

def time (s,c,l,t):
    return s / vel(c,l,t)

def analiticalPlot ():
    Rm = 7.0 * 1.0e+03
    Rc = 150.0
    Cm = 1.2
    c = 10.80800
    i = 0
    D = np.linspace(10,50,500)            # (um)
    S = [10.0,20.0,30.0,40.0,50.0]        # (cm)
    colorLines = ["black","red","blue","darkgreen","darkmagenta"]
    for s in S:
        t = time(s,c,lamb(Rm,Rc,D),tau(Rm,Cm))
        plt.plot(D,t,label=str(int(s))+" mm",color=colorLines[i],linewidth=2.0)
        i = i + 1

def aproximationPlot ():
    colorLines = ["black","red","blue","darkgreen","darkmagenta"]
    data = np.genfromtxt(open("1cm.dat","r"))
    plt.plot(data[:,0],data[:,1],"--",label="10 mm",color=colorLines[0],linewidth=2.0)
    data = np.genfromtxt(open("2cm.dat","r"))
    plt.plot(data[:,0],data[:,1],"--",label="20 mm",color=colorLines[1],linewidth=2.0)
    data = np.genfromtxt(open("3cm.dat","r"))
    plt.plot(data[:,0],data[:,1],"--",label="30 mm",color=colorLines[2],linewidth=2.0)
    data = np.genfromtxt(open("4cm.dat","r"))
    plt.plot(data[:,0],data[:,1],"--",label="40 mm",color=colorLines[3],linewidth=2.0)
    data = np.genfromtxt(open("5cm.dat","r"))
    plt.plot(data[:,0],data[:,1],"--",label="50 mm",color=colorLines[4],linewidth=2.0)

def main ():
    analiticalPlot()
    aproximationPlot()
    plt.title('Tempo de Ativacao - Analitico x Aproximacao')
    plt.ylabel('t (ms)')
    plt.xlabel('d (um)')
    plt.grid()
    plt.legend(loc=0,prop={'size': 11})
    plt.savefig("comp_time.pdf")
    print("[+] Figure save at 'comp_time.pdf'")

if __name__ == "__main__":
	main()