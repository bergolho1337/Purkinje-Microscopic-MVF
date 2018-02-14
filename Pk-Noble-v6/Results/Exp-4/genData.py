#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import numpy as np
import fnmatch
import subprocess
from matplotlib import pyplot

#Return the length of the Purkinje cell by using the name of the mesh
def getLength (meshName):
    	if (meshName == "pk1"):
		return 75.0
	if (meshName == "pk2"):
		return 100.0
	if (meshName == "pk3"):
		return 125.0
	if (meshName == "pk4"):
		return 150.0
	if (meshName == "pk5"):
		return 175.0
	if (meshName == "pk6"):
		return 200.0

# Return folders that use input alpha value
def getFolders (folders,alpha):
    arr = []
    string = "*a"+str(alpha)+"*"
    for folder in folders:
        if (fnmatch.fnmatch(folder,string)):
            arr.append(folder)
    return arr

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

my_xticks = [0,1,2,3]
pyplot.xticks(my_xticks)
pyplot.ylim([0,25])
#pyplot.xlabel(u"# bifurcações",fontsize=15)
#pyplot.ylabel(u"Tempo ativação (ms)",fontsize=15)
#pyplot.title(u'Tempo ativação x Número de bifurcações - (Noble)',fontsize=15)
pyplot.xlabel(u"# bifurcations",fontsize=15)
pyplot.ylabel(u"Activation Time (ms)",fontsize=15)
pyplot.title(u'Activation Time x Number of Bifurcations - (Noble)',fontsize=15)
pyplot.legend(loc=0,fontsize=14)
pyplot.grid()
pyplot.savefig("biff-at-Noble-EN.pdf")
#pyplot.show()
