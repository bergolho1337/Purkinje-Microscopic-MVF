# ===================================================================
# Generate the files for the Experiment Extra
# Experiment Extra - Analysis of the activation time due to bifurcations 
# ===================================================================
#!/bin/bash

if [ ! $# -eq 1 ]; then
    echo "---------------------------------------------------------------------------------"
	echo "Usage:> ./genExpExtra.sh --[option]"
    echo "--[option] = --sst / --solver"
    echo "---------------------------------------------------------------------------------"
	exit 1
fi

ALPHA=( 0.2 )
D1=( 0.001 0.0015 0.002 0.0025 0.003 0.0035 0.004 0.0045 0.005 )

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
            ./purkinje -t 0.01 2000 Meshes/Extra/cable2cm-dog.msh SteadyState/Extra/cable-d$d1 Plot/Extra/cable2cm-dog.plt $alpha $d1
            echo "------------------------------------------------------"
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
            mkdir -p Results/Extra/cable2cm-d$d1/Graphics
            ./purkinje -s 0.01 500 Meshes/Extra/cable2cm-dog.msh SteadyState/Extra/cable-d$d1 Plot/Extra/cable2cm-dog.plt $alpha $d1
            make plot
            mv Output/*.pdf Results/Extra/cable2cm-d$d1/Graphics
            mv Output/*.txt Results/Extra/cable2cm-d$d1 
            make clcResults
            echo "------------------------------------------------------"
        done
    done
else
    echo "---------------------------------------------------------------------------------"
    echo "ERROR ! Invalid option !"
    echo "---------------------------------------------------------------------------------"
fi