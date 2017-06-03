# ================================================================================================================
# Script para solucao automatica de um conjunto de malhas
# Cada simulacao eh feita para um tamanho fixo de fibra (FIBER_SIZE) e de elementos (NELEM)
# ************ Lembre-se de alterar a funcao setVelocityPoints() em "monodomainMVF.h" ************************
# ================================================================================================================
#!/bin/bash

PROGRAM_NAME="purkinjeMVF"
ARGS="0.1 1000"
MESH_FOLDER="Mesh-Networks-2"
STEADY_FOLDER="SteadyState-2"
MIN_MESH=1                                  # Numero minimo de malhas
MAX_MESH=3                                  # Numero maximo de malhas
FIBER_SIZE=0.50                                # Tamanho da fibra
NELEM=75                                   # Numero de elementos

echo "======= RUNNING BIFURCATION SIMULATION ======="
echo "FIBER SIZE = $FIBER_SIZE cm"

# Compile the source code if doesn't exist
if [ ! -f $PROGRAM_NAME ]; then
    echo "-----------------------------------------------------------------------------------------------------"
    echo "---> Compile source code ..."
    make clean
    make
    echo "-----------------------------------------------------------------------------------------------------"
fi

# Check if the Results directory already exist, if not make the directory
if [ ! -d Resultados ]; then
    echo "Making Resultados folder ..."
    mkdir Resultados
fi

# Run the simulation for each mesh
echo "-----------------------------------------------------------------------------------------------------"
for i in $(seq $MIN_MESH $MAX_MESH); do
    echo "------ Simulation $i ---------"
    ./$PROGRAM_NAME $ARGS $MESH_FOLDER/$FIBER_SIZE/E_$NELEM/test$i.msh $STEADY_FOLDER/e-$NELEM/steadystate$i.dat
    # Copy the results to the Resultados folder
    mkdir Resultados/Mesh_$i
    cp -r VTK Resultados/Mesh_$i
    mv ./velocity.txt Resultados/Mesh_$i/velocity$i.txt
    mv ./block.txt Resultados/Mesh_$i/block$i.txt
    mv ./data* Resultados/Mesh_$i/data$i.dat
    echo 
done

echo "-----------------------------------------------------------------------------------------------------"
    




