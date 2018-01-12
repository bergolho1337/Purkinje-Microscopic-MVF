import os
import numpy as np
import fnmatch
import subprocess
from matplotlib import pyplot

def getFolders (mesh,teta):
    output = []
    folders = os.listdir(mesh)
    folders.sort()
    for folder in folders:
        if (folder.find(teta) != -1):
            output.append(folder)
    return output

arr = ["sJMP", "cJMP"]
teta = ["-teta0.1-", "-teta0.15-", "-teta0.2-"]
perc = ["10 %", "15 %", "20 %"]
for mesh in arr:
    pyplot.clf()
    j = 0
    for t in teta:
        folders = getFolders(mesh,t)
        data = np.zeros((len(folders),2))

        # For each of these folders get the activation time of the terminal 
        i = 0
        for folder in folders:
            atime = np.genfromtxt(open(mesh+"/"+folder+"/time.txt","r"))
            data[i][0] = float(i)
            data[i][1] = float(atime)
            i = i + 1
        
        # Plot the data
        pyplot.plot(data[:,0],data[:,1],label=str(perc[j]),linewidth=2,marker='o')
        j = j + 1
    pyplot.ylim([0,50])
    pyplot.xlabel("# Nivel",fontsize=15)
    pyplot.ylabel("Tempo ativacao (ms)",fontsize=15)
    pyplot.title('Tempo ativacao x Nivel - Noble - '+str(mesh),fontsize=15)
    pyplot.legend(loc=0,fontsize=14)
    pyplot.grid()
    pyplot.savefig("lvl-at-"+str(mesh)+"-Noble.pdf")
    #pyplot.show()
