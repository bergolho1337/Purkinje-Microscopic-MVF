import sys
import os

def getCell (name):
    if (name == "alien"):
        return 100.0
    elif (name == "dog"):
        return 164.0
    elif (name == "orc"):
        return 200.0
    elif (name == "pig"):
        return 68.0

def getNumber (name):
    first = -1
    last = -1
    for i in range(len(name)):
        if (name[i].isdigit() or name[i] == '.'):
            if (first == -1):
                first = i
                last = i
            else:
                last = i
    return float(name[first:last+1])

def readFolder (folderName):
    folderArray = os.listdir(folderName)
    for folder in folderArray:
        token = folder.split('-')

        # Get size of the fiber
        lsize = getNumber(token[0])
        # Get size of the cell
        lcell = getCell(token[1])
        # Get alpha value
        alpha = getNumber(token[2])
        # Get diameter of the cell
        d1 = getNumber(token[3])

        # Take the required data from each simulation
        #tseg, tdelay, ttotal = processFile(folderName,folder)

        print("Folder name %s" % folder)
        print("lsize = %.5f" % lsize)
        print("lcell = %.5f" % lcell)
        print("alpha = %.5f" % alpha)
        print("d1 = %.5f\n" % d1)
        

def main():
    if (len(sys.argv) != 2):
        print("------------------------------------------------------")
        print("Usage:> python %s <folder_name>" % sys.argv[0])
        print("------------------------------------------------------")
        sys.exit(1) 
    readFolder(sys.argv[1])


if __name__ == "__main__":
    main()    