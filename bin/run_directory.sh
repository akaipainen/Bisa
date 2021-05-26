#!/bin/zsh
mkdir -p "output_results"

rm -rf "output"

for file in "$1/*.dat"; do
    /Users/alex/workspace/cern/Bisa/bin/Debug-macosx-x86_64/Commissioning/Commissioning /Users/alex/workspace/cern/Bisa/data/config.json "$file"
    file1="${file##*/}"
    mv /Users/alex/workspace/cern/Bisa/output /Users/alex/workspace/cern/Bisa/output_results/"${file1%%.*}"
done