#!/bin/bash

#FILES=$(find vall_alpha*)
#for FILE in $FILES; do
#    echo "[!] Saving figure $FILE"
#    python plotVelocity.py $FILE
#done

FILES=$(find delay_*)
for FILE in $FILES; do
    echo "[!] Saving figure $FILE"
    python plotDelay.py $FILE
done