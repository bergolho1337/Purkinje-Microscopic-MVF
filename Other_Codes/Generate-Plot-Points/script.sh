#!/bin/bash

if [ ! -f genPlot ]; then
    make
fi

./genPlot 0.5 ../../Pk-Noble-v5/Meshes/Exp-2/biff05cm-alien.msh biff05cm-alien.plt
./genPlot 0.5 ../../Pk-Noble-v5/Meshes/Exp-2/biff05cm-dog.msh biff05cm-dog.plt
./genPlot 0.5 ../../Pk-Noble-v5/Meshes/Exp-2/biff05cm-orc.msh biff05cm-orc.plt
./genPlot 0.5 ../../Pk-Noble-v5/Meshes/Exp-2/biff05cm-pig.msh biff05cm-pig.plt

./genPlot 1 ../../Pk-Noble-v5/Meshes/Exp-2/biff1cm-alien.msh biff1cm-alien.plt
./genPlot 1 ../../Pk-Noble-v5/Meshes/Exp-2/biff1cm-dog.msh biff1cm-dog.plt
./genPlot 1 ../../Pk-Noble-v5/Meshes/Exp-2/biff1cm-orc.msh biff1cm-orc.plt
./genPlot 1 ../../Pk-Noble-v5/Meshes/Exp-2/biff1cm-pig.msh biff1cm-pig.plt

./genPlot 2 ../../Pk-Noble-v5/Meshes/Exp-2/biff2cm-alien.msh biff2cm-alien.plt
./genPlot 2 ../../Pk-Noble-v5/Meshes/Exp-2/biff2cm-dog.msh biff2cm-dog.plt
./genPlot 2 ../../Pk-Noble-v5/Meshes/Exp-2/biff2cm-orc.msh biff2cm-orc.plt
./genPlot 2 ../../Pk-Noble-v5/Meshes/Exp-2/biff2cm-pig.msh biff2cm-pig.plt

./genPlot 5 ../../Pk-Noble-v5/Meshes/Exp-2/biff5cm-alien.msh biff5cm-alien.plt
./genPlot 5 ../../Pk-Noble-v5/Meshes/Exp-2/biff5cm-dog.msh biff5cm-dog.plt
./genPlot 5 ../../Pk-Noble-v5/Meshes/Exp-2/biff5cm-orc.msh biff5cm-orc.plt
./genPlot 5 ../../Pk-Noble-v5/Meshes/Exp-2/biff5cm-pig.msh biff5cm-pig.plt
