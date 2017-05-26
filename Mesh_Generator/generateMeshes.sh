# =========================================================================================================
# Script para gerar um conjunto de malhas de elementos finitos
# =========================================================================================================
#!/bin/bash

# Variables
MIN_SIZE=0.25
MAX_SIZE=0.25
MIN_MESH=1
MAX_MESH=40
MIN_ELEM=38
MAX_ELEM=38

if [ ! -e meshGenerator ]; then
    echo "========================================================================================"
    echo "[!] Compile the source code ..."
    make
    echo "========================================================================================"
fi

echo "========================================================================================"
if [ ! -d Mesh-Networks ]; then
    echo "[!] Making directory Mesh-Networks ..."
    mkdir Mesh-Networks
fi

echo "========================================================================================"
echo "[!] Generating meshes ..."
for i in $(LANG=en_US seq -f "%.2f" $MIN_SIZE 0.5 $MAX_SIZE); do
    #echo "----- Size = $i -----"
    mkdir ./Mesh-Networks/$i
    for j in $(seq $MIN_ELEM 50 $MAX_ELEM); do
        #echo "--------------------- Elements $j ------------------------"
        mkdir ./Mesh-Networks/$i/E_$j
        for k in $(seq $MIN_MESH $MAX_MESH); do
            ./meshGenerator $i $j ./Skeleton-Networks/$i/test$k.vtk ./Mesh-Networks/$i/E_$j/test$k.msh
            #echo "========================================= Mesh $k =========================================="
        done
    done
done

echo "========================================================================================"
exit 0
