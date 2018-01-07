# ===================================================================
# Generate the files for the Experiment 5
# Experiment 5 - Analysis of the activation time due to bifurcations
#                by decreasing the diameter of the Purkinje cell at
#                for each level of the network.
# teta = 10 % means that for the next level of the network we will
#        decrease the current diameter by 10% .
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
D1=( 0.003 )

if [ ! -f purkinje ]; then
    make
fi

if [ $1 == "--sst" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Steady-State simulation"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> teta = 0.1"
    for alpha in "${ALPHA[@]}"; do
        for d1 in "${D1[@]}"; do
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-5/pk1-teta0.1.msh SteadyState/Exp-5/pk1-cjmp-teta0.1-a$alpha-d$d1 Plot/Exp-5/pk1.plt $alpha $d1
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-5/pk2-teta0.1.msh SteadyState/Exp-5/pk2-cjmp-teta0.1-a$alpha-d$d1 Plot/Exp-5/pk2.plt $alpha $d1
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-5/pk3-teta0.1.msh SteadyState/Exp-5/pk3-cjmp-teta0.1-a$alpha-d$d1 Plot/Exp-5/pk3.plt $alpha $d1
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-5/pk4-teta0.1.msh SteadyState/Exp-5/pk4-cjmp-teta0.1-a$alpha-d$d1 Plot/Exp-5/pk4.plt $alpha $d1
            echo "------------------------------------------------------"
        done
    done
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> teta = 0.2"
    for alpha in "${ALPHA[@]}"; do
        for d1 in "${D1[@]}"; do
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-5/pk1-teta0.2.msh SteadyState/Exp-5/pk1-cjmp-teta0.2-a$alpha-d$d1 Plot/Exp-5/pk1.plt $alpha $d1
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-5/pk2-teta0.2.msh SteadyState/Exp-5/pk2-cjmp-teta0.2-a$alpha-d$d1 Plot/Exp-5/pk2.plt $alpha $d1
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-5/pk3-teta0.2.msh SteadyState/Exp-5/pk3-cjmp-teta0.2-a$alpha-d$d1 Plot/Exp-5/pk3.plt $alpha $d1
            echo "------------------------------------------------------"
            ./purkinje -t 0.01 2000 Meshes/Exp-5/pk4-teta0.2.msh SteadyState/Exp-5/pk4-cjmp-teta0.2-a$alpha-d$d1 Plot/Exp-5/pk4.plt $alpha $d1
            echo "------------------------------------------------------"
        done
    done
    echo "---------------------------------------------------------------------------------"
elif [ $1 == "--solver" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Solver simulation"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> teta = 0.1"
    for alpha in "${ALPHA[@]}"; do
        for d1 in "${D1[@]}"; do
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-5/cJMP/pk1-cjmp-teta0.1-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-5/pk1-teta0.1.msh SteadyState/Exp-5/pk1-cjmp-teta0.1-a$alpha-d$d1 Plot/Exp-5/pk1.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-5/cJMP/pk1-cjmp-teta0.1-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-5/cJMP/pk1-cjmp-teta0.1-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-5/cJMP/pk2-cjmp-teta0.1-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-5/pk2-teta0.1.msh SteadyState/Exp-5/pk2-cjmp-teta0.1-a$alpha-d$d1 Plot/Exp-5/pk2.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-5/cJMP/pk2-cjmp-teta0.1-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-5/cJMP/pk2-cjmp-teta0.1-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-5/cJMP/pk3-cjmp-teta0.1-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-5/pk3-teta0.1.msh SteadyState/Exp-5/pk3-cjmp-teta0.1-a$alpha-d$d1 Plot/Exp-5/pk3.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-5/cJMP/pk3-cjmp-teta0.1-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-5/cJMP/pk3-cjmp-teta0.1-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-5/cJMP/pk4-cjmp-teta0.1-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-5/pk4-teta0.1.msh SteadyState/Exp-5/pk4-cjmp-teta0.1-a$alpha-d$d1 Plot/Exp-5/pk4.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-5/cJMP/pk4-cjmp-teta0.1-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-5/cJMP/pk4-cjmp-teta0.1-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
        done
    done
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> teta = 0.2"
    for alpha in "${ALPHA[@]}"; do
        for d1 in "${D1[@]}"; do
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-5/cJMP/pk1-cjmp-teta0.2-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-5/pk1-teta0.2.msh SteadyState/Exp-5/pk1-cjmp-teta0.2-a$alpha-d$d1 Plot/Exp-5/pk1.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-5/cJMP/pk1-cjmp-teta0.2-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-5/cJMP/pk1-cjmp-teta0.2-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-5/cJMP/pk2-cjmp-teta0.2-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-5/pk2-teta0.2.msh SteadyState/Exp-5/pk2-cjmp-teta0.2-a$alpha-d$d1 Plot/Exp-5/pk2.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-5/cJMP/pk2-cjmp-teta0.2-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-5/cJMP/pk2-cjmp-teta0.2-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-5/cJMP/pk3-cjmp-teta0.2-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-5/pk3-teta0.2.msh SteadyState/Exp-5/pk3-cjmp-teta0.2-a$alpha-d$d1 Plot/Exp-5/pk3.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-5/cJMP/pk3-cjmp-teta0.2-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-5/cJMP/pk3-cjmp-teta0.2-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
            mkdir -p Results/Exp-5/cJMP/pk4-cjmp-teta0.2-a$alpha-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Exp-5/pk4-teta0.2.msh SteadyState/Exp-5/pk4-cjmp-teta0.2-a$alpha-d$d1 Plot/Exp-5/pk4.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Exp-5/cJMP/pk4-cjmp-teta0.2-a$alpha-d$d1/Graphics
            mv Output/*.txt Results/Exp-5/cJMP/pk4-cjmp-teta0.2-a$alpha-d$d1 
            make clcResults
            echo "------------------------------------------------------"
        done
    done
    echo "---------------------------------------------------------------------------------"
else
    echo "---------------------------------------------------------------------------------"
    echo "ERROR ! Invalid option !"
    echo "---------------------------------------------------------------------------------"
fi