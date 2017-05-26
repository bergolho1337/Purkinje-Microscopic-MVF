# =========================================================================================================
# Script para gerar um conjunto de moldes para a malha de elementos finitos
# =========================================================================================================
#!/bin/bash

# Variables
MIN_SIZE=0.253333333333
MAX_SIZE=0.253333333333
C_MESH=1
MAX_MESH=40

if [ ! -e skeletonMesh ]; then
    echo "========================================================================================"
    echo "[!] Compile the source code ..."
    make
    echo "========================================================================================"
fi

echo "========================================================================================"
if [ ! -d Skeleton-Networks ]; then
    echo "[!] Making directory Skeleton-Networks ..."
    mkdir Skeleton-Networks
fi

echo "[!] Generating skeleton meshes ..."
for i in $(LANG=en_US seq -f "%.2f" $MIN_SIZE 0.5 $MAX_SIZE); do
    echo "----- Size = $i -----"
    mkdir ./Skeleton-Networks/$i
    for j in $(seq $MIN_MESH $MAX_MESH); do
        ./skeletonMesh $i $j ./Skeleton-Networks/$i/test$j.vtk
    done
done

echo "========================================================================================"
exit 0


