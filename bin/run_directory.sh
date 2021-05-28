#!/bin/zsh
mkdir -p "output_results"

rm -rf "output"

for file in "$1"/*.dat; do
    bin/Debug-macosx/Commissioning/Commissioning config.json "$file" output
    file1="${file##*/}"
    mv /Users/alex/workspace/cern/Bisa/output /Users/alex/workspace/cern/Bisa/output_results/"${file1%%.*}"
done