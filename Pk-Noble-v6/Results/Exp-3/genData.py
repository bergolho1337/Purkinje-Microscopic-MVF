import os
import numpy as np
import fnmatch
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

alpha = [0.1, 0.2, 0.3, 0.4, 0.5]
folders = os.listdir('2cm')
folders.sort()

# For each value of alpha
for a in alpha:
    # Get the folders that use that alpha
    arr = getFolders(folders,a)
    data = np.zeros((len(arr),2))
    # For each of these folders get the diameter and the delay values 
    # and store on a matrix
    i = 0
    for folder in arr:
        delay = np.genfromtxt(open("./2cm/"+folder+"/delay.txt","r"))
        tokens = folder.split('-')
        h = getLength(tokens[0])
        data[i][0] = float(h)
        data[i][1] = float(delay)
        i = i + 1
    # Plot the data
    #pyplot.clf()
    pyplot.ylim([0,40])
    pyplot.xlabel("h ("+r'$\mu$ m'+")",fontsize=15)
    pyplot.ylabel("Atraso (ms)",fontsize=15)
    pyplot.title('Atraso x h - Noble',fontsize=15)
    pyplot.plot(data[:,0],data[:,1],label=r'$\gamma$=' + str(a),linewidth=2)
pyplot.legend(loc=0,fontsize=15)
pyplot.grid()
pyplot.savefig("alpha-h.pdf")
#pyplot.show()
