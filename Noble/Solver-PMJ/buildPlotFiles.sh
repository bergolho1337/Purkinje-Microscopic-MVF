#!/bin/bash

ALPHA=( 0.6875 1.375 2.55 )
D1=( 0.001 0.002 0.004 )
CELL_NAME=( "alien" "dog" "orc" "pig" )

echo "[!] Building plot files ..."

for alpha in "${ALPHA[@]}"; do
    for d1 in "${D1[@]}"; do
       FILES=($(find . -name "v.txt" | grep alpha_$alpha-d1_$d1 | sort))
       INDEX=0
       for FILE in ${FILES[@]}; do
           cut -f2 -d ' ' $FILE > v-$alpha-$d1-${CELL_NAME[$INDEX]}.txt
           let INDEX=INDEX+1
        done
    done
done


for alpha in "${ALPHA[@]}"; do
    for d1 in "${D1[@]}"; do
        FILES=($(ls | grep $alpha-$d1 | sort))
        paste 0.txt ${FILES[@]} > vall-alpha$alpha-d$d1.txt 
    done
done

rm v-*

