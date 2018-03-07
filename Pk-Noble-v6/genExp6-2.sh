# ===================================================================
# Generate the files for the Experiment 6
# Experiment 6 - Analysis of the activation time due to bifurcations
#                by decreasing the diameter of the Purkinje cell at
#                and the stimulated volume.
#                for each level of the network.
# ===================================================================
#!/bin/bash

if [ ! $# -eq 1 ]; then
    echo "---------------------------------------------------------------------------------"
	echo "Usage:> ./genExp5.sh --[option]"
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
    echo "[!] Running with:> delta = 0.9"
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk1-teta0.1.msh SteadyState/Exp-5/pk1-cjmp-teta0.9-a0.4-d0.003 Plot/Exp-5/pk1.plt 0.4 0.003
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk2-teta0.1.msh SteadyState/Exp-5/pk2-cjmp-teta0.9-a0.2-d0.003 Plot/Exp-5/pk2.plt 0.2 0.003
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk3-teta0.1.msh SteadyState/Exp-5/pk3-cjmp-teta0.9-a0.1-d0.003 Plot/Exp-5/pk3.plt 0.1 0.003
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk4-teta0.1.msh SteadyState/Exp-5/pk4-cjmp-teta0.9-a0.05-d0.003 Plot/Exp-5/pk4.plt 0.05 0.003
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> delta = 0.75"
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk1-teta0.15.msh SteadyState/Exp-5/pk1-cjmp-teta0.75-a0.4-d0.003 Plot/Exp-5/pk1.plt 0.4 0.003
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk2-teta0.15.msh SteadyState/Exp-5/pk2-cjmp-teta0.75-a0.2-d0.003 Plot/Exp-5/pk2.plt 0.2 0.003
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk3-teta0.15.msh SteadyState/Exp-5/pk3-cjmp-teta0.75-a0.1-d0.003 Plot/Exp-5/pk3.plt 0.1 0.003
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk4-teta0.15.msh SteadyState/Exp-5/pk4-cjmp-teta0.75-a0.05-d0.003 Plot/Exp-5/pk4.plt 0.05 0.003
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> delta = 0.5"
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk1-teta0.2.msh SteadyState/Exp-5/pk1-cjmp-teta0.5-a0.4-d0.003 Plot/Exp-5/pk1.plt 0.4 0.003
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk2-teta0.2.msh SteadyState/Exp-5/pk2-cjmp-teta0.5-a0.2-d0.003 Plot/Exp-5/pk2.plt 0.2 0.003
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk3-teta0.2.msh SteadyState/Exp-5/pk3-cjmp-teta0.5-a0.1-d0.003 Plot/Exp-5/pk3.plt 0.1 0.003
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-5/pk4-teta0.2.msh SteadyState/Exp-5/pk4-cjmp-teta0.5-a0.05-d0.003 Plot/Exp-5/pk4.plt 0.05 0.003
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
elif [ $1 == "--solver" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Solver simulation"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> delta = 0.9"
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk1-cjmp-teta0.9-a0.4-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk1-teta0.1.msh SteadyState/Exp-5/pk1-cjmp-teta0.9-a0.4-d0.003 Plot/Exp-5/pk1.plt 0.4 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk1-cjmp-teta0.9-a0.4-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk1-cjmp-teta0.9-a0.4-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk2-cjmp-teta0.9-a0.2-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk2-teta0.1.msh SteadyState/Exp-5/pk2-cjmp-teta0.9-a0.2-d0.003 Plot/Exp-5/pk2.plt 0.2 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk2-cjmp-teta0.9-a0.2-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk2-cjmp-teta0.9-a0.2-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk3-cjmp-teta0.9-a0.1-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk3-teta0.1.msh SteadyState/Exp-5/pk3-cjmp-teta0.9-a0.1-d0.003 Plot/Exp-5/pk3.plt 0.1 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk3-cjmp-teta0.9-a0.1-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk3-cjmp-teta0.9-a0.1-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk4-cjmp-teta0.9-a0.05-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk4-teta0.1.msh SteadyState/Exp-5/pk4-cjmp-teta0.9-a0.05-d0.003 Plot/Exp-5/pk4.plt 0.05 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk4-cjmp-teta0.9-a0.05-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk4-cjmp-teta0.9-a0.05-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> teta = 0.75"
    mkdir -p Results/Exp-5/cJMP/pk1-cjmp-teta0.75-a0.4-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk1-teta0.1.msh SteadyState/Exp-5/pk1-cjmp-teta0.75-a0.4-d0.003 Plot/Exp-5/pk1.plt 0.4 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk1-cjmp-teta0.75-a0.4-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk1-cjmp-teta0.75-a0.4-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk2-cjmp-teta0.75-a0.2-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk2-teta0.1.msh SteadyState/Exp-5/pk2-cjmp-teta0.75-a0.2-d0.003 Plot/Exp-5/pk2.plt 0.2 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk2-cjmp-teta0.75-a0.2-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk2-cjmp-teta0.75-a0.2-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk3-cjmp-teta0.75-a0.1-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk3-teta0.1.msh SteadyState/Exp-5/pk3-cjmp-teta0.75-a0.1-d0.003 Plot/Exp-5/pk3.plt 0.1 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk3-cjmp-teta0.75-a0.1-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk3-cjmp-teta0.75-a0.1-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk4-cjmp-teta0.75-a0.05-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk4-teta0.1.msh SteadyState/Exp-5/pk4-cjmp-teta0.75-a0.05-d0.003 Plot/Exp-5/pk4.plt 0.05 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk4-cjmp-teta0.75-a0.05-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk4-cjmp-teta0.75-a0.05-d0.003 
    make clcResults
    echo "---------------------------------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> teta = 0.5"
    mkdir -p Results/Exp-5/cJMP/pk1-cjmp-teta0.5-a0.4-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk1-teta0.1.msh SteadyState/Exp-5/pk1-cjmp-teta0.5-a0.4-d0.003 Plot/Exp-5/pk1.plt 0.4 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk1-cjmp-teta0.5-a0.4-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk1-cjmp-teta0.5-a0.4-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk2-cjmp-teta0.5-a0.2-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk2-teta0.1.msh SteadyState/Exp-5/pk2-cjmp-teta0.5-a0.2-d0.003 Plot/Exp-5/pk2.plt 0.2 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk2-cjmp-teta0.5-a0.2-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk2-cjmp-teta0.5-a0.2-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk3-cjmp-teta0.5-a0.1-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk3-teta0.1.msh SteadyState/Exp-5/pk3-cjmp-teta0.5-a0.1-d0.003 Plot/Exp-5/pk3.plt 0.1 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk3-cjmp-teta0.5-a0.1-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk3-cjmp-teta0.5-a0.1-d0.003 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-5/cJMP/pk4-cjmp-teta0.5-a0.05-d0.003/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-5/pk4-teta0.1.msh SteadyState/Exp-5/pk4-cjmp-teta0.5-a0.05-d0.003 Plot/Exp-5/pk4.plt 0.05 0.003
    make plot
    mv Output/*.pdf Results/Exp-5/cJMP/pk4-cjmp-teta0.5-a0.05-d0.003/Graphics
    mv Output/*.txt Results/Exp-5/cJMP/pk4-cjmp-teta0.5-a0.05-d0.003 
    make clcResults
    echo "---------------------------------------------------------------------------------"
else
    echo "---------------------------------------------------------------------------------"
    echo "ERROR ! Invalid option !"
    echo "---------------------------------------------------------------------------------"
fi