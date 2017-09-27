#!/bin/bash

if [ ! $# -eq 1 ]; then
    echo "Usage:> ./genTable.sh <FOLDER>"
    exit 1
fi 

FILES=$(ls $1)
COMMAND=""
COMMAND2=""
for FILE in $FILES; do
    COMMAND="$COMMAND $1/$FILE/v.txt"
    COMMAND2="$COMMAND2 $1/$FILE/delay.txt" 
done

paste $COMMAND > vall$1.txt
paste $COMMAND2 > delay$1.txt

COMMAND=""
mkdir -p Times
for FILE in $(ls $1); do
	#echo $1/$FILE >> out
	# Cable
	grep t1 $1/$FILE/velocity.txt | tail -n 1 | cut -d ' ' -f 3 >> Times/$FILE.txt
	grep t2 $1/$FILE/velocity.txt | tail -n 1 | cut -d ' ' -f 3 >> Times/$FILE.txt
	# Bifurcation
        #grep t1 $1/$FILE/velocity.txt | tail -n 2 | cut -d ' ' -f 3 >> Times/$FILE.txt
        #grep t2 $1/$FILE/velocity.txt | tail -n 2 | cut -d ' ' -f 3 >> Times/$FILE.txt
	COMMAND="$COMMAND Times/$FILE.txt"
	#echo >> times$1.txt
done

paste $COMMAND > times$1.txt
rm -r Times
