#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import numpy as np
import fnmatch
import subprocess
from matplotlib import pyplot

data1 = np.genfromtxt(open("v-delta0.9.dat","r"))
data2 = np.genfromtxt(open("v-delta0.8.dat","r"))
data3 = np.genfromtxt(open("v-delta0.7.dat","r"))

pyplot.clf()
my_xticks = [0,1,2,3]
pyplot.xticks(my_xticks)
pyplot.plot(data1[:,0],data1[:,1],"--",label="90 %",linewidth=2,color="darkred",marker='o')
pyplot.plot(data1[:,0],data2[:,1],"--",label="80 %",linewidth=2,color="darkgreen",marker='^')
pyplot.plot(data1[:,0],data3[:,1],"--",label="70 %",linewidth=2,color="darkblue",marker='s')
pyplot.plot(data1[:,0],data1[:,2],label="90 %",linewidth=2,color="darkred",marker='o')
pyplot.plot(data2[:,0],data2[:,2],label="80 %",linewidth=2,color="darkgreen",marker='^')
pyplot.plot(data3[:,0],data3[:,2],label="70 %",linewidth=2,color="darkblue",marker='s')
pyplot.ylim([0,4.0])
pyplot.xlabel(u"Nível",fontsize=15)
pyplot.ylabel(u"Velocidade propagação (m/s)",fontsize=15)
pyplot.title(u'Velocidade propagação x Nível - (LiRudy)',fontsize=15)
#pyplot.xlabel(u"# Bifurcations",fontsize=15)
#pyplot.ylabel(u"Propagation Velocity (m/s)",fontsize=15)
#pyplot.title(u'Propagation Velocity x Level - (LiRudy)',fontsize=15)
pyplot.legend(loc=0,fontsize=14,ncol=2)
pyplot.grid()
pyplot.savefig("lvl-velocity-LiRudy-revision.pdf")

