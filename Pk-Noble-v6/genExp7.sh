# ===================================================================
# Generate the files for the Experiment 7
# Experiment 7 - Analysis of the activation time due to bifurcations
#                by decreasing the diameter of the Purkinje cell at
#                for each level of the network (previously was the Exp-5)
# ===================================================================
#!/bin/bash

if [ ! $# -eq 2 ]; then
    echo "---------------------------------------------------------------------------------"
	echo "Usage:> ./genExp7.sh --[option] --[?PMJ]"
    echo "--[option] = --sst / --solver"
    echo "--[?PMJ] = --pmj / --notpmj"
    echo "---------------------------------------------------------------------------------"
	exit 1
fi

d1=0.003

if [ $2 == "--pmj" ]; then
    make clean; make -f MakefileCJMP
elif [ $2 == "--notpmj" ]; then
    make clean; make -f MakefileSJMP
else
    echo "---------------------------------------------------------------------------------"
    echo "ERROR ! Invalid option !"
    echo "---------------------------------------------------------------------------------"
fi

if [ $1 == "--sst" ]; then
    if [ $2 == "--pmj" ]; then
        echo "---------------------------------------------------------------------------------"
        echo "Running Steady-State simulation"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.9"
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk1-delta0.9.msh SteadyState/Exp-7/pk1-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk2-delta0.9.msh SteadyState/Exp-7/pk2-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk3-delta0.9.msh SteadyState/Exp-7/pk3-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk4-delta0.9.msh SteadyState/Exp-7/pk4-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.8"
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk1-delta0.8.msh SteadyState/Exp-7/pk1-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk2-delta0.8.msh SteadyState/Exp-7/pk2-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk3-delta0.8.msh SteadyState/Exp-7/pk3-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk4-delta0.8.msh SteadyState/Exp-7/pk4-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.7"
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk1-delta0.7.msh SteadyState/Exp-7/pk1-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk2-delta0.7.msh SteadyState/Exp-7/pk2-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk3-delta0.7.msh SteadyState/Exp-7/pk3-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk4-delta0.7.msh SteadyState/Exp-7/pk4-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
    elif [ $2 == "--notpmj" ]; then
        echo "---------------------------------------------------------------------------------"
        echo "Running Steady-State simulation"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.9"
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk1-delta0.9.msh SteadyState/Exp-7/pk1-sjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk2-delta0.9.msh SteadyState/Exp-7/pk2-sjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk3-delta0.9.msh SteadyState/Exp-7/pk3-sjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk4-delta0.9.msh SteadyState/Exp-7/pk4-sjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.8"
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk1-delta0.8.msh SteadyState/Exp-7/pk1-sjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk2-delta0.8.msh SteadyState/Exp-7/pk2-sjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk3-delta0.8.msh SteadyState/Exp-7/pk3-sjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk4-delta0.8.msh SteadyState/Exp-7/pk4-sjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.7"
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk1-delta0.7.msh SteadyState/Exp-7/pk1-sjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk2-delta0.7.msh SteadyState/Exp-7/pk2-sjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk3-delta0.7.msh SteadyState/Exp-7/pk3-sjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        echo "------------------------------------------------------"
        ./purkinje -t 0.01 2000 Meshes/Exp-7/pk4-delta0.7.msh SteadyState/Exp-7/pk4-sjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
    else
        echo "---------------------------------------------------------------------------------"
        echo "ERROR ! Invalid option !"
        echo "---------------------------------------------------------------------------------"
    fi
elif [ $1 == "--solver" ]; then
    if [ $2 == "--pmj" ]; then
        echo "---------------------------------------------------------------------------------"
        echo "Running Solver simulation"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.9"
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk1-cjmp-delta0.9-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk1-delta0.9.msh SteadyState/Exp-7/pk1-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk1-cjmp-delta0.9-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk1-cjmp-delta0.9-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk2-cjmp-delta0.9-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk2-delta0.9.msh SteadyState/Exp-7/pk2-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk2-cjmp-delta0.9-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk2-cjmp-delta0.9-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk3-cjmp-delta0.9-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk3-delta0.9.msh SteadyState/Exp-7/pk3-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk3-cjmp-delta0.9-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk3-cjmp-delta0.9-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk4-cjmp-delta0.9-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk4-delta0.9.msh SteadyState/Exp-7/pk4-cjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk4-cjmp-delta0.9-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk4-cjmp-delta0.9-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.8"
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk1-cjmp-delta0.8-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk1-delta0.8.msh SteadyState/Exp-7/pk1-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk1-cjmp-delta0.8-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk1-cjmp-delta0.8-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk2-cjmp-delta0.8-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk2-delta0.8.msh SteadyState/Exp-7/pk2-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk2-cjmp-delta0.8-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk2-cjmp-delta0.8-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk3-cjmp-delta0.8-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk3-delta0.8.msh SteadyState/Exp-7/pk3-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk3-cjmp-delta0.8-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk3-cjmp-delta0.8-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk4-cjmp-delta0.8-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk4-delta0.8.msh SteadyState/Exp-7/pk4-cjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk4-cjmp-delta0.8-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk4-cjmp-delta0.8-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.7"
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk1-cjmp-delta0.7-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk1-delta0.7.msh SteadyState/Exp-7/pk1-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk1-cjmp-delta0.7-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk1-cjmp-delta0.7-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk2-cjmp-delta0.7-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk2-delta0.7.msh SteadyState/Exp-7/pk2-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk2-cjmp-delta0.7-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk2-cjmp-delta0.7-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk3-cjmp-delta0.7-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk3-delta0.7.msh SteadyState/Exp-7/pk3-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk3-cjmp-delta0.7-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk3-cjmp-delta0.7-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/cJMP/pk4-cjmp-delta0.7-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk4-delta0.7.msh SteadyState/Exp-7/pk4-cjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/cJMP/pk4-cjmp-delta0.7-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/cJMP/pk4-cjmp-delta0.7-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
    elif [ $2 == "--notpmj" ]; then
        echo "---------------------------------------------------------------------------------"
        echo "Running Solver simulation"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.9"
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk1-sjmp-delta0.9-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk1-delta0.9.msh SteadyState/Exp-7/pk1-sjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk1-sjmp-delta0.9-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk1-sjmp-delta0.9-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk2-sjmp-delta0.9-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk2-delta0.9.msh SteadyState/Exp-7/pk2-sjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk2-sjmp-delta0.9-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk2-sjmp-delta0.9-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk3-sjmp-delta0.9-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk3-delta0.9.msh SteadyState/Exp-7/pk3-sjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk3-sjmp-delta0.9-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk3-sjmp-delta0.9-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk4-sjmp-delta0.9-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk4-delta0.9.msh SteadyState/Exp-7/pk4-sjmp-delta0.9-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk4-sjmp-delta0.9-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk4-sjmp-delta0.9-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.8"
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk1-sjmp-delta0.8-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk1-delta0.8.msh SteadyState/Exp-7/pk1-sjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk1-sjmp-delta0.8-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk1-sjmp-delta0.8-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk2-sjmp-delta0.8-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk2-delta0.8.msh SteadyState/Exp-7/pk2-sjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk2-sjmp-delta0.8-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk2-sjmp-delta0.8-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk3-sjmp-delta0.8-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk3-delta0.8.msh SteadyState/Exp-7/pk3-sjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk3-sjmp-delta0.8-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk3-sjmp-delta0.8-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk4-sjmp-delta0.8-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk4-delta0.8.msh SteadyState/Exp-7/pk4-sjmp-delta0.8-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk4-sjmp-delta0.8-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk4-sjmp-delta0.8-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
        echo "[!] Running with:> delta = 0.7"
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk1-sjmp-delta0.7-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk1-delta0.7.msh SteadyState/Exp-7/pk1-sjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk1.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk1-sjmp-delta0.7-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk1-sjmp-delta0.7-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk2-sjmp-delta0.7-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk2-delta0.7.msh SteadyState/Exp-7/pk2-sjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk2.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk2-sjmp-delta0.7-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk2-sjmp-delta0.7-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk3-sjmp-delta0.7-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk3-delta0.7.msh SteadyState/Exp-7/pk3-sjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk3.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk3-sjmp-delta0.7-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk3-sjmp-delta0.7-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        mkdir -p Results/Exp-7/sJMP/pk4-sjmp-delta0.7-a0.4-d$d1/Graphics
        ./purkinje -s 0.01 500 Meshes/Exp-7/pk4-delta0.7.msh SteadyState/Exp-7/pk4-sjmp-delta0.7-a0.4-d$d1 Plot/Exp-7/pk4.plt 0.4 $d1
        make plot
        mv Output/*.pdf Results/Exp-7/sJMP/pk4-sjmp-delta0.7-a0.4-d$d1/Graphics
        mv Output/*.txt Results/Exp-7/sJMP/pk4-sjmp-delta0.7-a0.4-d$d1 
        make clcResults
        echo "------------------------------------------------------"
        echo "---------------------------------------------------------------------------------"
    else
        echo "---------------------------------------------------------------------------------"
        echo "ERROR ! Invalid option !"
        echo "---------------------------------------------------------------------------------"
    fi
else
    echo "---------------------------------------------------------------------------------"
    echo "ERROR ! Invalid option !"
    echo "---------------------------------------------------------------------------------"
fi