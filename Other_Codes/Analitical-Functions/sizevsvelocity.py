#!/usr/bin/python
# -*- coding: UTF-8 -*-
# ------------------------------------------------------------------------------------------------------------------------------------
# Program that shows the relation between the size of cell and propagation velocity	
# ------------------------------------------------------------------------------------------------------------------------------------

import matplotlib.pyplot as plt
import numpy as np
import sys

def main ():
    if (len(sys.argv) != 2):
        print("Usage:> %s <filename>" % sys.argv[0])
        sys.exit(1)

    data = np.genfromtxt(open(sys.argv[1],"r"))
    plt.plot(data[:,0],data[:,1],"-",label="aprox")
    plt.title(u'Tamanho da célula x Velocidade de Propagação',size=15)
    plt.ylim([0.0,3.0])
    plt.ylabel('v (m/s)')
    plt.xlabel('h (um)')
    plt.grid()
    plt.legend(loc=0,fontsize=15)
    plt.savefig("L-v.pdf")
    print("[+] Figure save at 'L-v.pdf'")

if __name__ == "__main__":
	main()
