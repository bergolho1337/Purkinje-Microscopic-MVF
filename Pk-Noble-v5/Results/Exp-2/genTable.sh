#!/bin/bash

if [ ! $# -eq 2 ]; then
    echo "Usage:> ./genTable.sh <FOLDER> <ID>"
    echo "<ID> = 1(Cable) 2(Bifurcation)"
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
	if [ $2 -eq 1 ]; then
		grep t1 $1/$FILE/velocity.txt | tail -n 1 | cut -d ' ' -f 3 >> Times/$FILE.txt
        	grep t2 $1/$FILE/velocity.txt | tail -n 1 | cut -d ' ' -f 3 >> Times/$FILE.txt
	fi
	# Bifurcation
	if [ $2 -eq 2 ]; then
		grep t1 $1/$FILE/velocity.txt | tail -n 7 | cut -d ' ' -f 3 >> Times/$FILE.txt
       		grep t2 $1/$FILE/velocity.txt | tail -n 7 | cut -d ' ' -f 3 >> Times/$FILE.txt
	fi
	# Bifurcation
        COMMAND="$COMMAND Times/$FILE.txt"
	#echo >> times$1.txt
done

paste $COMMAND > times$1.txt
rm -r Times
