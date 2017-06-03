# =========================================================================================================
# Script para gerar um conjunto de malhas de elementos finitos
# =========================================================================================================
#!/bin/bash

# Variables
MESH_FOLDER="Mesh-Networks-2"
SKELETON_FOLDER="Skeleton-Networks-2"
MIN_SIZE=1.00
MAX_SIZE=1.00
OFFSET_SIZE=0.5
MIN_MESH=1
MAX_MESH=3
MIN_ELEM=150
MAX_ELEM=150
OFFSET_ELEM=75

if [ ! -e meshGenerator ]; then
    echo "========================================================================================"
    echo "[!] Compile the source code ..."
    make
    echo "========================================================================================"
fi

echo "========================================================================================"
if [ ! -d $MESH_FOLDER ]; then
    echo "[!] Making directory $MESH_FOLDER ..."
    mkdir $MESH_FOLDER
fi

echo "========================================================================================"
echo "[!] Generating meshes ..."
for i in $(LANG=en_US seq -f "%.2f" $MIN_SIZE $OFFSET_SIZE $MAX_SIZE); do
    echo "----- Size = $i -----"
    mkdir ./$MESH_FOLDER/$i
    for j in $(seq $MIN_ELEM $OFFSET_ELEM $MAX_ELEM); do
        echo "--------------------- Elements $j ------------------------"
        mkdir ./$MESH_FOLDER/$i/E_$j
        for k in $(seq $MIN_MESH $MAX_MESH); do
            ./meshGenerator $i $j ./$SKELETON_FOLDER/$i/test$k.vtk ./$MESH_FOLDER/$i/E_$j/test$k.msh
            #echo "========================================= Mesh $k =========================================="
        done
    done
done

echo "[!] Copying $MESH_FOLDER to the Steady_State_Calculator folder"
cp -r $MESH_FOLDER ../Steady_State_Calculator/

echo "========================================================================================"

echo "[!] Copying $MESH_FOLDER to the Solver folder"
cp -r $MESH_FOLDER ../Solver/

echo "========================================================================================"
exit 0
