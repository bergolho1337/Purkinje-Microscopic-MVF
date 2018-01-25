# Script to generate the file with all the results from Experiment 1 and 2.
# The output file will be 'results.dat' and will be formatted as:
# -------------------------------------------------------------------------------
# lsize     lcell       d1      alpha       tseg        tdelay      ttotal   
# -------------------------------------------------------------------------------

#!/bin/bash

# Experiment 1 results
python getInfoFolder.py Exp-1/05cm; mv results.dat results-1-05cm.dat
python getInfoFolder.py Exp-1/1cm; mv results.dat results-1-1cm.dat
python getInfoFolder.py Exp-1/2cm; mv results.dat results-1-2cm.dat
python getInfoFolder.py Exp-1/5cm; mv results.dat results-1-5cm.dat

# Experiment 2 results
python getInfoFolder.py Exp-2/05cm; mv results.dat results-2-05cm.dat
python getInfoFolder.py Exp-2/1cm; mv results.dat results-2-1cm.dat
python getInfoFolder.py Exp-2/2cm; mv results.dat results-2-2cm.dat
python getInfoFolder.py Exp-2/5cm; mv results.dat results-2-5cm.dat

cat results-1-05cm.dat results-2-05cm.dat results-1-1cm.dat results-2-1cm.dat results-1-2cm.dat results-2-2cm.dat results-1-5cm.dat results-2-5cm.dat > results.dat
rm results-1* results-2*