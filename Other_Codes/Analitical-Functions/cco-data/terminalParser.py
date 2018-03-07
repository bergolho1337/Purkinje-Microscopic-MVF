

inFile = open("term.txt","r")
outFile = open("term2.txt","w")

cont = 0
for term in inFile:
    cont = cont + 1

inFile.close()
inFile = open("term.txt","r")

outFile.write("%d\n" % cont)
for term in inFile:
    outFile.write("%d " % (int(term)-5))
outFile.write("\n")