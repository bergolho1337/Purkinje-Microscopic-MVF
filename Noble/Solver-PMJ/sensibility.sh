#!/bin/bash
CELL_NAME=( "alien" "dog" "orc" "pig" )
ALPHA=( 0.6875 1.375 2.55 )
D1=( 0.001 0.002 0.004 )

for alpha in "${ALPHA[@]}"; do
    for cn in "${CELL_NAME[@]}"; do
        for d1 in "${D1[@]}"; do
            mkdir Results/5cm-$cn-alpha_$alpha-d1_$d1
            make clean; make
            echo "------------------------------------------------------"
            echo "[!] Running with:> $cn, alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            ./solverMVF 0.1 1000 ../Data/Mesh/cable5cm_$cn.msh ../Data/SteadyState/5cm-$cn-alpha_$alpha-d1_$d1.sst $alpha $d1
            make plot
            mv Output/*.txt Results/5cm-$cn-alpha_$alpha-d1_$d1
            mv Output/*.pdf Results/5cm-$cn-alpha_$alpha-d1_$d1
            cp -r VTK Results/5cm-$cn-alpha_$alpha-d1_$d1
        done
    done
done

sh Results/5cm/buildPlotFiles.sh
sh Results/5cm/plot.sh
