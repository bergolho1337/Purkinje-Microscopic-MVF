#!/bin/bash

#echo "[!] Plotting velocity files ..."
#FILES=$(find vall-alpha*)
#for FILE in $FILES; do
#    echo "[!] Saving figure $FILE"
#    python plotVelocity.py $FILE
#done

echo "[!] Plotting delay files ..."
FILES=$(find delay_*)
for FILE in $FILES; do
    echo "[!] Saving figure $FILE"
    python plotDelay.py $FILE
done

#mv *.png Graficos
