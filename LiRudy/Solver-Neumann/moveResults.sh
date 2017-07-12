#!/bin/bash

if [ ! $# -eq 1 ]; then
    echo "================================================="
    echo "Usage:> ./moveResults.sh <out_result_folder>"
    echo "================================================="
    exit 1
fi

echo "[!] Moving Output for experiment '$1' to Results folder !"
mkdir Results/$1
mv Output/*.png Results/$1
mv Output/*.txt Results/$1
cp -r VTK Results/$1/