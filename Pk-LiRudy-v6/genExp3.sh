# ===================================================================
# Generate the files for the Experiment 3
# Experiment 3 - Analysis of the delay on PMJ vs length of Pk-cell 
# ===================================================================
#!/bin/bash

if [ ! $# -eq 1 ]; then
    echo "---------------------------------------------------------------------------------"
	echo "Usage:> ./genExp3.sh --[option]"
    echo "--[option] = --sst / --solver"
    echo "---------------------------------------------------------------------------------"
	exit 1
fi

ALPHA=( 0.1 0.2 0.3 0.4 0.5 )
D1=( 0.0055 )

if [ ! -f purkinje ]; then
    make
fi

if [ $1 == "--sst" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Steady-State simulation"
    echo "---------------------------------------------------------------------------------"
    for alpha in "${ALPHA[@]}"; do
        for d1 in "${D1[@]}"; do
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 75"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-3/pk1.msh SteadyState/Exp-3/pk1-a$alpha-d$d1 Plot/Exp-3/pk1.plt $alpha $d1
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 100"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-3/pk2.msh SteadyState/Exp-3/pk2-a$alpha-d$d1 Plot/Exp-3/pk2.plt $alpha $d1
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 125"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-3/pk3.msh SteadyState/Exp-3/pk3-a$alpha-d$d1 Plot/Exp-3/pk3.plt $alpha $d1
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 150"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-3/pk4.msh SteadyState/Exp-3/pk4-a$alpha-d$d1 Plot/Exp-3/pk4.plt $alpha $d1
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 175"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-3/pk5.msh SteadyState/Exp-3/pk5-a$alpha-d$d1 Plot/Exp-3/pk5.plt $alpha $d1
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 200"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-3/pk6.msh SteadyState/Exp-3/pk6-a$alpha-d$d1 Plot/Exp-3/pk6.plt $alpha $d1
        done
    done
elif [ $1 == "--solver" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Solver simulation"
    echo "---------------------------------------------------------------------------------"
    for alpha in "${ALPHA[@]}"; do
        for d1 in "${D1[@]}"; do
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 75"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-3/2cm/pk1-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-3/pk1.msh SteadyState/Exp-3/pk1-a$alpha-d$d1 Plot/Exp-3/pk1.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-3/2cm/pk1-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-3/2cm/pk1-a$alpha-d$d1; 
            make clcResults
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 100"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-3/2cm/pk2-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-3/pk2.msh SteadyState/Exp-3/pk2-a$alpha-d$d1 Plot/Exp-3/pk2.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-3/2cm/pk2-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-3/2cm/pk2-a$alpha-d$d1; 
            make clcResults
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 125"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-3/2cm/pk3-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-3/pk3.msh SteadyState/Exp-3/pk3-a$alpha-d$d1 Plot/Exp-3/pk3.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-3/2cm/pk3-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-3/2cm/pk3-a$alpha-d$d1; 
            make clcResults
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 150"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-3/2cm/pk4-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-3/pk4.msh SteadyState/Exp-3/pk4-a$alpha-d$d1 Plot/Exp-3/pk4.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-3/2cm/pk4-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-3/2cm/pk4-a$alpha-d$d1; 
            make clcResults
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 175"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-3/2cm/pk5-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-3/pk5.msh SteadyState/Exp-3/pk5-a$alpha-d$d1 Plot/Exp-3/pk5.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-3/2cm/pk5-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-3/2cm/pk5-a$alpha-d$d1; 
            make clcResults
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1, h = 200"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-3/2cm/pk6-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-3/pk6.msh SteadyState/Exp-3/pk6-a$alpha-d$d1 Plot/Exp-3/pk6.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-3/2cm/pk6-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-3/2cm/pk6-a$alpha-d$d1; 
            make clcResults
        done
    done
else
    echo "---------------------------------------------------------------------------------"
    echo "ERROR ! Invalid option !"
    echo "---------------------------------------------------------------------------------"
fi