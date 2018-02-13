# ========================================================================
# Run an experiment and copy the results to the MEGASync cloud folder
# ========================================================================

#!/bin/bash

#./genExp7.sh --sst --notpmj
#./genExp7.sh --sst --pmj
#cp ./SteadyState/Exp-7/* ~/MEGA/Resultados-Exp7

./genExp7.sh --solver --notpmj
./genExp7.sh --solver --pmj
cp -r ./Results/Exp-7/* ~/MEGA/Resultados-Exp7
