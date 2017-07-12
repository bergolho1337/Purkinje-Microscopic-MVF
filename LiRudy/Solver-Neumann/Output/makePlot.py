import os
import sys
import re
from matplotlib import pyplot
from pylab import genfromtxt  

def getDigits ():
	ids = []
	files = [f for f in os.listdir('.') if ".dat" in f]
	for file in files:
		ids.append(int(re.findall('\d+',file)[0]))
	ids.sort()
	return ids

def plotSolution (ids):
	i = 1
	# Plot graphics for the solution of the control volumes
	for id in ids:
		print("[!] Saving figure %d" % i)
		mat0 = genfromtxt("data"+str(id)+".dat")
		pyplot.plot(mat0[:,0], mat0[:,1], label = "Vm")
		pyplot.grid()
		pyplot.title(str("Volume = " + str(id) + " | Level = " + str(i)))
		pyplot.xlabel("Time (ms)")
		pyplot.ylabel("Transmembrane potential (mV)")
		pyplot.ylim([-100,60])
		pyplot.legend()
		pyplot.savefig(str("potential" + str(i) + ".png"))
		#pyplot.show()
		pyplot.clf()
		i = i + 1

def plotVelocity ():
    # Plot graphics for the propagation velocity
	print("[!] Saving velocity figure")
	mat0 = genfromtxt("v.txt")
	pyplot.plot(mat0[:,0], mat0[:,1], label = "v")
	pyplot.grid()
	pyplot.title("Propagation velocity x Level")
	pyplot.xlabel("Level of the tree")
	pyplot.ylabel("Velocity (cm/s)")
	pyplot.ylim([0,500])  
	pyplot.legend()
	pyplot.savefig("velocity_level.png")
	pyplot.clf()

def main ():
    	
	ids = getDigits()
	plotSolution(ids)
	plotVelocity()

if __name__ == "__main__":
    main()


