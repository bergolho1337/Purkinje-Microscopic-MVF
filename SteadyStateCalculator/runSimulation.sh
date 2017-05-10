# ==========================================================================================================
# Script para execucao automatica do SteadyState de um conjunto de malhas.
# Nesta versao se utiliza o estado estacionario da solucao anterior
# ==========================================================================================================

#!/bin/bash

# Variables
PROGRAM_NAME="steadystateMVF"                  # Nome do programa
ARGS="0.1 5000"                             # Argumentos do programa
MIN_ELEM=75                                # Numero minimo de elementos
MAX_ELEM=75                                # Numero maximo de elementos
MIN_MESH=1                                  # Numero minimo de malhas
MAX_MESH=13                                  # Numero maximo de malhas
FIBER_SIZE=0.50                             # Tamanho da fibra

echo "======= RUNNING STEADY STATE SIMULATION ======="
# Compilar o codigo fonte
if [ ! -f $PROGRAM_NAME ]; then
    echo "-----------------------------------------------------------------------------------------------------"
    echo "---> Compile source code ..."
    make
    echo "-----------------------------------------------------------------------------------------------------"
fi

# Criar os diretorios caso eles nao existam
if [ ! -d SteadyState ]; then
    echo "Making SteadyState folder ..."
    mkdir SteadyState
fi

if [ ! -d ./SteadyState/e-$MIN_ELEM ]; then
    echo "Making ./SteadyState/e-$MIN_ELEM folder ..."
    mkdir ./SteadyState/e-$MIN_ELEM
fi    

echo "-----------------------------------------------------------------------------------------------------"
echo ">>>>>>>>>>>>>>>>>>>>>>>>> Number of elements $MIN_ELEM <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"

# Rodar para a primeira malha
echo "------ SteadyState - Mesh $MIN_MESH ---------"
./$PROGRAM_NAME $ARGS Malhas/$FIBER_SIZE/E_$MIN_ELEM/test$MIN_MESH.msh $MIN_MESH
let MIN_MESH=MIN_MESH+1

# Rodar para as outras malhas utilizando o resultado da simulacao passada
for j in $(seq $MIN_MESH $MAX_MESH); do
    echo "------ SteadyState - Mesh $j ---------"
    ./$PROGRAM_NAME $ARGS Malhas/$FIBER_SIZE/E_$MIN_ELEM/test$j.msh $j ./steadystate$((j-1)).dat
done
# Move the data to the SteadyState folder
mv ./*.dat ./SteadyState/e-$MIN_ELEM/
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
echo "-----------------------------------------------------------------------------------------------------"


exit 0    
