# ===================================================================
# Generate the Solver files for the Experiment 2
# Experiment 2 - Analysis of the delay on PMJ vs diameter of Pk-cell 
# ===================================================================
#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "---------------------------------------------------------------------------------"
    echo "./genExp1.sh <FOLDER_NAME> <MESH_NAME> <EXP_NUMBER>"
    echo "<FOLDER_NAME> = '1cm' or 'Biff1cm'"
    echo "<MESH_NAME> = 'cable1cm' or 'biff1cm'"
    echo "<EXP_NUMBER> = 1(Experiment 1), 2(Experiment 2)" 
    echo "---------------------------------------------------------------------------------"
    exit 1
fi

FOLDER_NAME=$1
MESH_NAME=$2
EXP_NUMBER=$3

#CELL_NAME=( "alien" "dog" "orc" "pig" )
CELL_NAME=( "dog" )
ALPHA=( 0.1 0.2 0.3 0.4 0.5 )
D1=( 0.00525 0.0055 0.00575 0.006 )

if [ ! -f purkinje ]; then
    make
fi

if [ ! -d Results ]; then
    mkdir Results
fi

for alpha in "${ALPHA[@]}"; do
    for d1 in "${D1[@]}"; do
        for cn in "${CELL_NAME[@]}"; do
            echo "------------------------------------------------------"
            echo "[!] Running with:> $cn, alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            mkdir Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1; mkdir Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-$EXP_NUMBER/$MESH_NAME-$cn.msh SteadyState/Exp-$EXP_NUMBER/$MESH_NAME-$cn-a$alpha-d$d1 Plot/Exp-$EXP_NUMBER/$MESH_NAME-$cn.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1; 
            make clcResults
        done
    done
done
