# ===================================================================
# Generate the Solver files for the sensibility analysis for a
# specific mesh. 
# ===================================================================
#!/bin/bash
CELL_NAME=( "alien" "dog" "orc" "pig" )
ALPHA=( 0.6875 1.375 2.55 )
D1=( 0.001 0.002 0.004 )

if [ ! -f purkinje ]; then
    make
fi

if [ ! -d Results ]; then
    mkdir Results
fi

for alpha in "${ALPHA[@]}"; do
    for d1 in "${D1[@]}"; do
        for cn in "${CELL_NAME[@]}"; do
            echo "------------------------------------------------------"
            echo "[!] Running with:> $cn, alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            mkdir Results/Biff-5cm/biff5cm-$cn-a$alpha-d$d1; mkdir Results/Biff-5cm/biff5cm-$cn-a$alpha-d$d1/Graphics
            ./purkinje -s 0.1 1000 Meshes/biff5cm-$cn.msh SteadyState/Biff-5cm/biff5cm-$cn-a$alpha-d$d1 $alpha $d1
            make plot
            mv Output/*.pdf Results/Biff-5cm/biff5cm-$cn-a$alpha-d$d1/Graphics
            cp -r VTK Results/Biff-5cm/biff5cm-$cn-a$alpha-d$d1
            mv Output/delay.txt Results/Biff-5cm/biff5cm-$cn-a$alpha-d$d1; 
            mv Output/v.txt Results/Biff-5cm/biff5cm-$cn-a$alpha-d$d1
            mv Output/velocity.txt Results/Biff-5cm/biff5cm-$cn-a$alpha-d$d1
            rm -f Output/*.dat Output/*.txt Output/*.pdf
        done
    done
done
