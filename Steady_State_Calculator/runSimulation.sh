# ==========================================================================================================
# Script para execucao automatica do SteadyState de um conjunto de malhas.
# Esta versao soh funciona para malhas do tipo 1.
# Nesta versao se utiliza o estado estacionario da solucao anterior.
# ==========================================================================================================

#!/bin/bash

# Variables
PROGRAM_NAME="steadystateMVF"                  # Nome do programa
STEADY_FOLDER="SteadyState"                  # Nome do diretorio dos estados estacionarios
MESH_FOLDER="Mesh-Networks"                    # Nome do diretorio das malhas
ARGS="0.1 5000"                                # Argumentos do programa
MIN_ELEM=150                                    # Numero minimo de elementos
MAX_ELEM=150                                    # Numero maximo de elementos
MIN_MESH=1                                     # Numero minimo de malhas
MAX_MESH=2                                     # Numero maximo de malhas
FIBER_SIZE=1.00                                # Tamanho da fibra

echo "======= RUNNING STEADY STATE SIMULATION ======= FIBER TYPE 1 ========"
# Compilar o codigo fonte
if [ ! -f $PROGRAM_NAME ]; then
    echo "-----------------------------------------------------------------------------------------------------"
    echo "---> Compile source code ..."
    make
    echo "-----------------------------------------------------------------------------------------------------"
fi

# Criar os diretorios caso eles nao existam
if [ ! -d $STEADY_FOLDER ]; then
    echo "Making $STEADY_FOLDER folder ..."
    mkdir $STEADY_FOLDER
fi

if [ ! -d ./$STEADY_FOLDER/e-$MIN_ELEM ]; then
    echo "Making ./$STEADY_FOLDER/e-$MIN_ELEM folder ..."
    mkdir ./$STEADY_FOLDER/e-$MIN_ELEM
fi    

echo "-----------------------------------------------------------------------------------------------------"
echo ">>>>>>>>>>>>>>>>>>>>>>>>> Number of elements $MIN_ELEM <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"

# Rodar para a primeira malha
echo "------ SteadyState - Mesh $MIN_MESH ---------"
./$PROGRAM_NAME $ARGS $MESH_FOLDER/$FIBER_SIZE/E_$MIN_ELEM/test$MIN_MESH.msh $MIN_MESH
let MIN_MESH=MIN_MESH+1

# Rodar para as outras malhas utilizando o resultado da simulacao passada
for j in $(seq $MIN_MESH $MAX_MESH); do
    echo "------ SteadyState - Mesh $j ---------"
    ./$PROGRAM_NAME $ARGS $MESH_FOLDER/$FIBER_SIZE/E_$MIN_ELEM/test$j.msh $j ./steadystate$((j-1)).dat
done
# Move the data to the SteadyState folder
echo "[!] Moving the files to the $STEADY_FOLDER folder"
mv ./*.dat ./$STEADY_FOLDER/e-$MIN_ELEM/

# Copy the SteadyState folder to the Solver folder
echo "[!] Copying the $STEADY_FOLDER folder to the Solver folder"
cp -r ./$STEADY_FOLDER ../Solver

echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
echo "-----------------------------------------------------------------------------------------------------"

exit 0    
