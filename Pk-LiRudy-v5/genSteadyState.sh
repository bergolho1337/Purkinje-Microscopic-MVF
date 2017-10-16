# ===================================================================
# Generate the Steady State files for the sensibility analysis for a
# specific mesh. 
# ===================================================================
#!/bin/bash

if [ ! $# -eq 2 ]; then
    echo "---------------------------------------------------------------------------------"
	echo "Usage:> ./genSteadyState <MESH_NAME> <EXP_NUMBER>"
    echo "<MESH_NAME> = 'cable1cm' or 'biff1cm'"
    echo "<EXP_NUMBER> = 1(Experiment 1), 2(Experiment 2)"
    echo "Experiment 1 = Distance from source to sink is 2 times the length of a fiber"
    echo "Experiment 2 = Distance from source to sink is 3 times the length of a fiber"
    echo "---------------------------------------------------------------------------------"
	exit 1
fi

MESH_NAME=$1
EXP_NUMBER=$2
CELL_NAME=( "alien" "dog" "orc" "pig" )
ALPHA=( 0.34375 )
D1=( 0.001 0.002 0.004 )

if [ ! -f purkinje ]; then
    make
fi

for alpha in "${ALPHA[@]}"; do
    for d1 in "${D1[@]}"; do
        for cn in "${CELL_NAME[@]}"; do
            echo "------------------------------------------------------"
            echo "[!] Running with:> $cn, alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            # The SteadyState files will written outside the folder
            ./purkinje -t 0.01 5000 Meshes/Exp-$EXP_NUMBER/$MESH_NAME-$cn.msh SteadyState/Exp-$EXP_NUMBER/$MESH_NAME-$cn-a$alpha-d$d1 Plot/Exp-$EXP_NUMBER/$MESH_NAME-$cn.plt $alpha $d1
        done
    done
done
