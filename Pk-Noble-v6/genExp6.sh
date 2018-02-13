# ===================================================================
# Generate the files for the Experiment 6
# Experiment 6 - Analysis of the activation time due to bifurcations
#                by decreasing the diameter of the Purkinje cell at
#                for each level of the network and the myocardium
#                volume
# ===================================================================
#!/bin/bash

if [ ! $# -eq 1 ]; then
    echo "---------------------------------------------------------------------------------"
	echo "Usage:> ./genExp6.sh --[option]"
    echo "--[option] = --sst / --solver"
    echo "---------------------------------------------------------------------------------"
	exit 1
fi

d1=0.003

if [ ! -f purkinje ]; then
    make
fi

if [ $1 == "--sst" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Steady-State simulation"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> delta = 0.9"
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk1-delta0.9.msh SteadyState/Exp-6/pk1-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-6/pk1.plt 0.4 $d1
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk2-delta0.9.msh SteadyState/Exp-6/pk2-cjmp-delta0.9-a0.2-d$d1 Plot/Exp-6/pk2.plt 0.2 $d1
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk3-delta0.9.msh SteadyState/Exp-6/pk3-cjmp-delta0.9-a0.1-d$d1 Plot/Exp-6/pk3.plt 0.1 $d1
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk4-delta0.9.msh SteadyState/Exp-6/pk4-cjmp-delta0.9-a0.05-d$d1 Plot/Exp-6/pk4.plt 0.05 $d1
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> delta = 0.8"
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk1-delta0.8.msh SteadyState/Exp-6/pk1-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-6/pk1.plt 0.4 $d1
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk2-delta0.8.msh SteadyState/Exp-6/pk2-cjmp-delta0.8-a0.2-d$d1 Plot/Exp-6/pk2.plt 0.2 $d1
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk3-delta0.8.msh SteadyState/Exp-6/pk3-cjmp-delta0.8-a0.1-d$d1 Plot/Exp-6/pk3.plt 0.1 $d1
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk4-delta0.8.msh SteadyState/Exp-6/pk4-cjmp-delta0.8-a0.05-d$d1 Plot/Exp-6/pk4.plt 0.05 $d1
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> delta = 0.7"
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk1-delta0.7.msh SteadyState/Exp-6/pk1-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-6/pk1.plt 0.4 $d1
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk2-delta0.7.msh SteadyState/Exp-6/pk2-cjmp-delta0.7-a0.2-d$d1 Plot/Exp-6/pk2.plt 0.2 $d1
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk3-delta0.7.msh SteadyState/Exp-6/pk3-cjmp-delta0.7-a0.1-d$d1 Plot/Exp-6/pk3.plt 0.1 $d1
    echo "------------------------------------------------------"
    ./purkinje -t 0.01 2000 Meshes/Exp-6/pk4-delta0.7.msh SteadyState/Exp-6/pk4-cjmp-delta0.7-a0.05-d$d1 Plot/Exp-6/pk4.plt 0.05 $d1
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
elif [ $1 == "--solver" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Solver simulation"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> delta = 0.9"
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk1-cjmp-delta0.9-a0.4-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk1-delta0.9.msh SteadyState/Exp-6/pk1-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-6/pk1.plt 0.4 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk1-cjmp-delta0.9-a0.4-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk1-cjmp-delta0.9-a0.4-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk2-cjmp-delta0.9-a0.2-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk2-delta0.9.msh SteadyState/Exp-6/pk2-cjmp-delta0.9-a0.2-d$d1 Plot/Exp-6/pk2.plt 0.2 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk2-cjmp-delta0.9-a0.2-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk2-cjmp-delta0.9-a0.2-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk3-cjmp-delta0.9-a0.1-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk3-delta0.9.msh SteadyState/Exp-6/pk3-cjmp-delta0.9-a0.1-d$d1 Plot/Exp-6/pk3.plt 0.1 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk3-cjmp-delta0.9-a0.1-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk3-cjmp-delta0.9-a0.1-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk4-cjmp-delta0.9-a0.05-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk4-delta0.9.msh SteadyState/Exp-6/pk4-cjmp-delta0.9-a0.05-d$d1 Plot/Exp-6/pk4.plt 0.05 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk4-cjmp-delta0.9-a0.05-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk4-cjmp-delta0.9-a0.05-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> delta = 0.8"
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk1-cjmp-delta0.8-a0.4-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk1-delta0.8.msh SteadyState/Exp-6/pk1-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-6/pk1.plt 0.4 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk1-cjmp-delta0.8-a0.4-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk1-cjmp-delta0.8-a0.4-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk2-cjmp-delta0.8-a0.2-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk2-delta0.8.msh SteadyState/Exp-6/pk2-cjmp-delta0.8-a0.2-d$d1 Plot/Exp-6/pk2.plt 0.2 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk2-cjmp-delta0.8-a0.2-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk2-cjmp-delta0.8-a0.2-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk3-cjmp-delta0.8-a0.1-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk3-delta0.8.msh SteadyState/Exp-6/pk3-cjmp-delta0.8-a0.1-d$d1 Plot/Exp-6/pk3.plt 0.1 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk3-cjmp-delta0.8-a0.1-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk3-cjmp-delta0.8-a0.1-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk4-cjmp-delta0.8-a0.05-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk4-delta0.8.msh SteadyState/Exp-6/pk4-cjmp-delta0.8-a0.05-d$d1 Plot/Exp-6/pk4.plt 0.05 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk4-cjmp-delta0.8-a0.05-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk4-cjmp-delta0.8-a0.05-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
    echo "[!] Running with:> delta = 0.7"
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk1-cjmp-delta0.7-a0.4-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk1-delta0.7.msh SteadyState/Exp-6/pk1-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-6/pk1.plt 0.4 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk1-cjmp-delta0.7-a0.4-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk1-cjmp-delta0.7-a0.4-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk2-cjmp-delta0.7-a0.2-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk2-delta0.7.msh SteadyState/Exp-6/pk2-cjmp-delta0.7-a0.2-d$d1 Plot/Exp-6/pk2.plt 0.2 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk2-cjmp-delta0.7-a0.2-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk2-cjmp-delta0.7-a0.2-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk3-cjmp-delta0.7-a0.1-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk3-delta0.7.msh SteadyState/Exp-6/pk3-cjmp-delta0.7-a0.1-d$d1 Plot/Exp-6/pk3.plt 0.1 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk3-cjmp-delta0.7-a0.1-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk3-cjmp-delta0.7-a0.1-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    mkdir -p Results/Exp-6/cJMP/pk4-cjmp-delta0.7-a0.05-d$d1/Graphics
    ./purkinje -s 0.01 500 Meshes/Exp-6/pk4-delta0.7.msh SteadyState/Exp-6/pk4-cjmp-delta0.7-a0.05-d$d1 Plot/Exp-6/pk4.plt 0.05 $d1
    make plot
    mv Output/*.pdf Results/Exp-6/cJMP/pk4-cjmp-delta0.7-a0.05-d$d1/Graphics
    mv Output/*.txt Results/Exp-6/cJMP/pk4-cjmp-delta0.7-a0.05-d$d1 
    make clcResults
    echo "------------------------------------------------------"
    echo "---------------------------------------------------------------------------------"
else
    echo "---------------------------------------------------------------------------------"
    echo "ERROR ! Invalid option !"
    echo "---------------------------------------------------------------------------------"
fi