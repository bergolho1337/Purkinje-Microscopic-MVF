import os
import numpy as np

def calculateExecutionTime (modelName):
    FILES = os.listdir(".")
    # Find all the files with the model name and sort them by the core number
    TIMEFILES = []
    for FILE in FILES:
        if (FILE.find(modelName) != -1):
            TIMEFILES.append(FILE)
    TIMEFILES.sort()

    # For each file calculate the mean execution time using Numpy
    EXECTIMES = []
    for TIMEFILE in TIMEFILES:
        data = np.genfromtxt(open(TIMEFILE))
        meanTime = np.mean(data)
        EXECTIMES.append(meanTime)

    print("-------------------------------------------------------------------------------------------------------------------")
    print("Execution times for the %s model\n" % (modelName))
    i = 1
    SERIALTIME = EXECTIMES[0]
    for EXECTIME in EXECTIMES:
        SPEEDUP = SERIALTIME / EXECTIME
        EFFICIENCY = SPEEDUP / float(i)
        print("Core %d -- Time: %.2lf -- Speedup = %.2lf -- Efficiency = %.2lf\n" % (i,EXECTIME,SPEEDUP,EFFICIENCY))
        i = i * 2
    print("-------------------------------------------------------------------------------------------------------------------")
    

def main ():
    calculateExecutionTime("Noble")
    calculateExecutionTime("LiRudy")

if __name__ == "__main__":
    main()
