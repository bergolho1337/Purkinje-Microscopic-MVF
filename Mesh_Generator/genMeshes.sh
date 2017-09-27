#!/bin/bash

if [ ! -f meshGenerator ]; then
	make
fi

SK_FOLDER="Skeletons"
MSH_FOLDER="Meshes"

# Bifurcation Iterative
./meshGenerator $SK_FOLDER/biffIter05cm.vtk $MSH_FOLDER/biffIter05cm-alien.msh --alien
./meshGenerator $SK_FOLDER/biffIter05cm.vtk $MSH_FOLDER/biffIter05cm-dog.msh --dog
./meshGenerator $SK_FOLDER/biffIter05cm.vtk $MSH_FOLDER/biffIter05cm-orc.msh --orc
./meshGenerator $SK_FOLDER/biffIter05cm.vtk $MSH_FOLDER/biffIter05cm-pig.msh --pig

./meshGenerator $SK_FOLDER/biffIter1cm.vtk $MSH_FOLDER/biffIter1cm-alien.msh --alien
./meshGenerator $SK_FOLDER/biffIter1cm.vtk $MSH_FOLDER/biffIter1cm-dog.msh --dog
./meshGenerator $SK_FOLDER/biffIter1cm.vtk $MSH_FOLDER/biffIter1cm-orc.msh --orc
./meshGenerator $SK_FOLDER/biffIter1cm.vtk $MSH_FOLDER/biffIter1cm-pig.msh --pig

./meshGenerator $SK_FOLDER/biffIter2cm.vtk $MSH_FOLDER/biffIter2cm-alien.msh --alien
./meshGenerator $SK_FOLDER/biffIter2cm.vtk $MSH_FOLDER/biffIter2cm-dog.msh --dog
./meshGenerator $SK_FOLDER/biffIter2cm.vtk $MSH_FOLDER/biffIter2cm-orc.msh --orc
./meshGenerator $SK_FOLDER/biffIter2cm.vtk $MSH_FOLDER/biffIter2cm-pig.msh --pig

./meshGenerator $SK_FOLDER/biffIter5cm.vtk $MSH_FOLDER/biffIter5cm-alien.msh --alien
./meshGenerator $SK_FOLDER/biffIter5cm.vtk $MSH_FOLDER/biffIter5cm-dog.msh --dog
./meshGenerator $SK_FOLDER/biffIter5cm.vtk $MSH_FOLDER/biffIter5cm-orc.msh --orc
./meshGenerator $SK_FOLDER/biffIter5cm.vtk $MSH_FOLDER/biffIter5cm-pig.msh --pig
