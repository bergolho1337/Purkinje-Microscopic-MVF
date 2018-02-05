# ======================================================================
# Generate the files for the Experiment 1.5
# Experiment 1.5 - Analysis of the Propagation velocity vs Conductivity 
# ======================================================================
#!/bin/bash

if [ ! $# -eq 1 ]; then
    echo "---------------------------------------------------------------------------------"
	echo "Usage:> ./genExp1-5.sh --[option]"
    echo "--[option] = --sst / --solver"
    echo "---------------------------------------------------------------------------------"
	exit 1
fi

ALPHA=( 0.1 )
D1=( 0.0035 )
SIGMA=( 0.0001 0.00053333 0.00096667 0.0014 0.00183333 0.00226667 0.0027 0.00313333 0.00356667 0.004 )

if [ ! -f purkinje ]; then
    make
fi

if [ $1 == "--sst" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Steady-State simulation"
    echo "---------------------------------------------------------------------------------"
    for sigma in "${SIGMA[@]}"; do
        echo "------------------------------------------------------"
        echo "[!] Running with:> sigma = $sigma"
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-1/cable2cm-dog.msh SteadyState/Exp-1-5/cable2cm-$sigma Plot/Exp-1/cable2cm-dog.plt $ALPHA $D1 $sigma
    done
elif [ $1 == "--solver" ]; then
    echo "---------------------------------------------------------------------------------"
    echo "Running Solver simulation"
    for sigma in "${SIGMA[@]}"; do
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> sigma = $sigma"
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-1-5/2cm/cable2cm-$sigma/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-1/cable2cm-dog.msh SteadyState/Exp-1-5/cable2cm-$sigma Plot/Exp-1/cable2cm-dog.plt $ALPHA $D1 $sigma
        make plot
        mv Output/*.pdf Results/Exp-1-5/2cm/cable2cm-$sigma/Graphics
        mv Output/*.txt Results/Exp-1-5/2cm/cable2cm-$sigma; 
        make clcResults
    done
else
    echo "---------------------------------------------------------------------------------"
    echo "ERROR ! Invalid option !"
    echo "---------------------------------------------------------------------------------"
fi