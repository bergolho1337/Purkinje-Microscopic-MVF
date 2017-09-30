# ===================================================================
# Generate the Steady State files for the sensibility analysis for a
# specific mesh. 
# ===================================================================
#!/bin/bash

if [ ! $# -eq 1 ]; then
	echo "Usage:> ./genSteadyState <MESH_NAME>"
	exit 1
fi

MESH_NAME=$1
CELL_NAME=( "alien" "dog" "orc" "pig" )
ALPHA=( 0.6875 1.375 2.55 )
D1=( 0.001 0.002 0.004 )

if [ ! -f purkinje ]; then
    make
fi

for alpha in "${ALPHA[@]}"; do
    for d1 in "${D1[@]}"; do
        for cn in "${CELL_NAME[@]}"; do
            echo "------------------------------------------------------"
            echo "[!] Running with:> $cn, alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 5000 Meshes/$MESH_NAME-$cn.msh SteadyState/$MESH_NAME-$cn-a$alpha-d$d1 Plot/$MESH_NAME-$cn.plt $alpha $d1
        done
    done
done
