# ===================================================================
# Generate the Steady State files for the sensibility analysis for a
# specific mesh. 
# ===================================================================
#!/bin/bash
MESH_NAME="cable5cm"
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
            ./purkinje -t 0.1 5000 Meshes/biff5cm-$cn.msh SteadyState/biff5cm-$cn-a$alpha-d$d1 $alpha $d1
        done
    done
done
