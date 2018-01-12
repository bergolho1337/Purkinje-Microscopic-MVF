import os
import numpy as np
import fnmatch
import subprocess
from matplotlib import pyplot

arr = ["sJMP", "cJMP"]
for mesh in arr:
    folders = os.listdir(mesh)
    folders.sort()
    data = np.zeros((len(folders),2))

    # For each of these folders get the activation time of the terminal 
    # and store on a matrix along with the number of bifurcations for each network
    i = 0
    for folder in folders:
        atime = np.genfromtxt(open(mesh+"/"+folder+"/time.txt","r"))
        data[i][0] = float(i)
        data[i][1] = float(atime)
        i = i + 1
    # Plot the data
    #pyplot.clf()
    pyplot.plot(data[:,0],data[:,1],label=str(mesh),linewidth=2,marker='o')

pyplot.ylim([0,25])
pyplot.xlabel("# bifurcacoes",fontsize=15)
pyplot.ylabel("Tempo ativacao (ms)",fontsize=15)
pyplot.title('Tempo ativacao x Numero de bifurcacoes - LiRudy',fontsize=15)
pyplot.legend(loc=0,fontsize=14)
pyplot.grid()
pyplot.savefig("biff-at-LiRudy.pdf")
#pyplot.show()
