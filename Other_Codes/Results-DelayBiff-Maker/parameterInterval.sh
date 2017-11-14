# Generate the first part of the output file for the results from the delay on the bifurcation
# Format:
#   <size_fiber> <size_cell> <alpha> <d1> <num_biff>

#!/bin/bash
SIZE_FIBER_EXP1=( 1.0 2.0 4.0 10.0 )
SIZE_FIBER_EXP2=( 1.5 3.0 6.0 15.0 )
SIZE_CELL=( 100.0 164.0 200.0 )
ALPHA=( 0.171875 0.34375 0.6875 )
D1=( 0.002 0.0025 0.003 0.0035 0.004 )

for sf in "${SIZE_FIBER_EXP1[@]}"; do
    for sc in "${SIZE_CELL[@]}"; do
        for alpha in "${ALPHA[@]}"; do
            for d1 in "${D1[@]}"; do
                echo "$sf $sc $d1 $alpha 1" >> out1
            done
        done
    done
done

for sf in "${SIZE_FIBER_EXP2[@]}"; do
    for sc in "${SIZE_CELL[@]}"; do
        for alpha in "${ALPHA[@]}"; do
            for d1 in "${D1[@]}"; do
                echo "$sf $sc $d1 $alpha 2" >> out1
            done
        done
    done
done
