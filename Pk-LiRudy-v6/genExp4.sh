# ===================================================================
# Generate the files for the Experiment 4
# Experiment 4 - Analysis of the activation time due to bifurcations 
# ===================================================================
#!/bin/bash

if [ ! $# -eq 1 ]; then
    echo "---------------------------------------------------------------------------------"
	echo "Usage:> ./genExp4.sh --[option]"
    echo "--[option] = --sst / --solver"
    echo "---------------------------------------------------------------------------------"
	exit 1
fi

ALPHA=( 0.2 )
D1=( 0.004 )

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
            echo "[!] Running with:> alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-4/pk1.msh SteadyState/Exp-4/pk1-cjmp-a$alpha-d$d1 Plot/Exp-4/pk1.plt $alpha $d1
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-4/pk2.msh SteadyState/Exp-4/pk2-cjmp-a$alpha-d$d1 Plot/Exp-4/pk2.plt $alpha $d1
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-4/pk3.msh SteadyState/Exp-4/pk3-cjmp-a$alpha-d$d1 Plot/Exp-4/pk3.plt $alpha $d1
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-4/pk4.msh SteadyState/Exp-4/pk4-cjmp-a$alpha-d$d1 Plot/Exp-4/pk4.plt $alpha $d1
        done
    done
elif [ $1 == "--solver" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Solver simulation"
    echo "---------------------------------------------------------------------------------"
    for alpha in "${ALPHA[@]}"; do
        for d1 in "${D1[@]}"; do
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-4/cJMP/pk1-cjmp-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-4/pk1.msh SteadyState/Exp-4/pk1-cjmp-a$alpha-d$d1 Plot/Exp-4/pk1.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-4/cJMP/pk1-cjmp-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-4/cJMP/pk1-cjmp-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-4/cJMP/pk2-cjmp-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-4/pk2.msh SteadyState/Exp-4/pk2-cjmp-a$alpha-d$d1 Plot/Exp-4/pk2.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-4/cJMP/pk2-cjmp-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-4/cJMP/pk2-cjmp-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-4/cJMP/pk3-cjmp-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-4/pk3.msh SteadyState/Exp-4/pk3-cjmp-a$alpha-d$d1 Plot/Exp-4/pk3.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-4/cJMP/pk3-cjmp-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-4/cJMP/pk3-cjmp-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
            echo "[!] Running with:> alpha = $alpha, d1 = $d1"
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-4/cJMP/pk4-cjmp-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-4/pk4.msh SteadyState/Exp-4/pk4-cjmp-a$alpha-d$d1 Plot/Exp-4/pk4.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-4/cJMP/pk4-cjmp-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-4/cJMP/pk4-cjmp-a$alpha-d$d1 
            make clcResults
        done
    done
else
    echo "---------------------------------------------------------------------------------"
    echo "ERROR ! Invalid option !"
    echo "---------------------------------------------------------------------------------"
fi