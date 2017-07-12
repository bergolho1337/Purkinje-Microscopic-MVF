# =========================================================================================================
# Script para gerar um conjunto de moldes para a malha de elementos finitos
# =========================================================================================================
#!/bin/bash

# Variables
NETWORK_FOLDER="Skeleton-Networks-2"
TYPE=2
MIN_SIZE=0.50
MAX_SIZE=1.00
C_MESH=1
MAX_MESH=3

if [ ! -e skeletonMesh ]; then
    echo "========================================================================================"
    echo "[!] Compile the source code ..."
    make
    echo "========================================================================================"
fi

echo "========================================================================================"
if [ ! -d $NETWORK_FOLDER ]; then
    echo "[!] Making directory $NETWORK_FOLDER ..."
    mkdir $NETWORK_FOLDER
fi

echo "[!] Generating skeleton meshes ..."
for i in $(LANG=en_US seq -f "%.2f" $MIN_SIZE 0.5 $MAX_SIZE); do
    echo "----- Size = $i -----"
    mkdir ./$NETWORK_FOLDER/$i
    for j in $(seq $MIN_MESH $MAX_MESH); do
        ./skeletonMesh $i $j $TYPE ./$NETWORK_FOLDER/$i/test$j.vtk
    done
done

echo "[!] Copying '$NETWORK_FOLDER' to the Mesh_Generator folder"
cp -r $NETWORK_FOLDER ../Mesh_Generator/

echo "========================================================================================"
exit 0


