import matplotlib.pyplot as plt
import numpy as np
import sys

def main ():
    if (len(sys.argv) != 2):
        print("Usage:> %s <filename>" % sys.argv[0])
        sys.exit(1)

    data = np.genfromtxt(open(sys.argv[1],"r"))
    plt.plot(data[:,0],data[:,1],"-",label="aprox")
    plt.title('Tamanho da celula x Velocidade de Propagacao')
    plt.ylim([0.0,3.0])
    plt.ylabel('v (m/s)')
    plt.xlabel('L (um)')
    plt.grid()
    plt.legend(loc=0)
    plt.savefig("L-v.pdf")
    print("[+] Figure save at 'L-v.pdf'")

if __name__ == "__main__":
	main()
