#!/bin/bash

for FILE in $(ls Meshes); do
    echo Meshes/$FILE
    ./MeshInfo/fiber Meshes/$FILE
    echo
done
