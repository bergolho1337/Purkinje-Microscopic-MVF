# ===================================================================
# Generate the Solver files for the sensibility analysis for a
# specific mesh. 
# ===================================================================
#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "---------------------------------------------------------------------------------"
    echo "./genSolver <FOLDER_NAME> <MESH_NAME> <EXP_NUMBER>"
    echo "<FOLDER_NAME> = '1cm' or 'Biff1cm'"
    echo "<MESH_NAME> = 'cable1cm' or 'biff1cm'"
    echo "<EXP_NUMBER> = 1(Experiment 1), 2(Experiment 2)"
    echo "Experiment 1 = Distance from source to sink is 2 times the length of a fiber"
    echo "Experiment 2 = Distance from source to sink is 3 times the length of a fiber"
    echo "---------------------------------------------------------------------------------"
    exit 1
fi

FOLDER_NAME=$1
MESH_NAME=$2
EXP_NUMBER=$3

CELL_NAME=( "alien" "dog" "orc" "pig" )
#CELL_NAME=( "dog" )
ALPHA=( 0.34375 0.6875 1.375 )
D1=( 0.001 0.002 0.004 )

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
            mkdir Results/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1; mkdir Results/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-$EXP_NUMBER/$MESH_NAME-$cn.msh SteadyState/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1 Plot/Exp-$EXP_NUMBER/$MESH_NAME-$cn.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1/Graphics
            cp -r VTK Results/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1
            mv Output/delay.txt Results/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1; 
            mv Output/v.txt Results/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1
            mv Output/velocity.txt Results/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1
            rm -f Output/*.dat Output/*.txt Output/*.pdf
        done
    done
done
