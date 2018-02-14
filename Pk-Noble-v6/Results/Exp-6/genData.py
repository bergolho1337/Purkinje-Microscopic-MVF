#!/usr/bin/python
# -*- coding: UTF-8 -*-
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

arr = ["cJMP"]
teta = ["-delta0.9-", "-delta0.8-", "-delta0.7-"]
perc = ["90 %", "80 %", "70 %"]
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
    my_xticks = [0,1,2,3]
    pyplot.xticks(my_xticks)
    pyplot.ylim([0,50])
    #pyplot.xlabel(u"# Nível",fontsize=15)
    #pyplot.ylabel(u"Tempo ativação (ms)",fontsize=15)
    #pyplot.title(u'Tempo ativação x Nível - (Noble)',fontsize=15)
    pyplot.xlabel(u"# Bifurcations",fontsize=15)
    pyplot.ylabel(u"Activation Time (ms)",fontsize=15)
    pyplot.title(u'Activation Time x Level - (Noble)',fontsize=15)
    pyplot.legend(loc=0,fontsize=14)
    pyplot.grid()
    pyplot.savefig("lvl-at-"+str(mesh)+"-Noble-EN.pdf")
    #pyplot.show()
