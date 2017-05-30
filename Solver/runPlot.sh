# ================================================================================================================
# Script para plotar os graficos dos arquivos data.dat no intervalo [MIN_MESH,MAX_MESH] 
# Cada simulacao eh feita para um tamanho fixo de fibra (FIBER_SIZE)
# ================================================================================================================
#!/bin/bash

MIN_MESH=1
MAX_MESH=10
SIZE_FIBER="1.00"

if [ $SIZE_FIBER == "1.00" ]; then
    VOLUME_ID=258
elif [ $SIZE_FIBER == "0.50" ]; then
    VOLUME_ID=129
else
    VOLUME_ID=-1
fi

if [ $VOLUME_ID -ne -1 ]; then
    for i in $(seq $MIN_MESH $MAX_MESH); do
        echo "[!] Writing figure for mesh $i ..."
        python makePlot.py Resultados/Mesh_$i/data$i.dat $VOLUME_ID
        mv *.png Resultados/Mesh_$i
    done
else
    echo "[-] Nao existe este tamanho de fibra !"
    exit 0
fi