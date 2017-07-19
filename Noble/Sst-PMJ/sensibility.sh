#!/bin/bash
CELL_NAME=( "alien" "dog" "orc" "pig" )
ALPHA=( 0.6875 1.375 2.55 )
D1=( 0.001 0.002 0.004 )

for alpha in "${ALPHA[@]}"; do
    for cn in "${CELL_NAME[@]}"; do
        for d1 in "${D1[@]}"; do
            make clean; make
            echo "------------------------------------------------------"
            echo "[!] Running with:> $cn, alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            ./steadystateMVF 0.1 5000 ../Data/Mesh/cable5cm_$cn.msh ../Data/SteadyState/5cm-$cn-alpha_$alpha-d1_$d1.sst $alpha $d1
        done
    done
done