#!/bin/zsh
mkdir -p "output_results"

rm -rf "output"

for file in "$1"/*.dat; do
    bin/Debug-macosx/Commissioning/Commissioning config.json "$file" output
    file1="${file##*/}"
    mv output output_results/"${file1%%.*}"
done