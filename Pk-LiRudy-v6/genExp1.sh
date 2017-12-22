# ===================================================================
# Generate the Solver files for the Experiment-1
# Experiment-1 = Propagation velocity vs diameter of Pk-cell
#                Activation time vs diameter of Pk-cell
# ===================================================================
#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "---------------------------------------------------------------------------------"
    echo "./genExp1.sh <FOLDER_NAME> <MESH_NAME> <EXP_NUMBER>"
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

#CELL_NAME=( "alien" "dog" "orc" "pig" )
CELL_NAME=( "dog" )
ALPHA=( 0.171875 )
D1=( 0.001 0.0015 0.002 0.0025 0.003 0.0035 0.004 0.0045 0.005 )

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
            VAR=$(echo $d1 $(head -n 1 Output/time.txt) | awk '{printf "%.10f %.10f\n",$1*10000.0,$2}')
            echo "$VAR" >> $FOLDER_NAME.dat
            make plot
            mv Output/*.pdf Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1/Graphics
            cp -r VTK Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1
            mv Output/delay.txt Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1; 
            mv Output/v.txt Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1
            mv Output/time.txt Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1
            mv Output/velocity.txt Results/Exp-$EXP_NUMBER/$FOLDER_NAME/$MESH_NAME-$cn-a$alpha-d$d1
            make clcResults
        done
    done
done
