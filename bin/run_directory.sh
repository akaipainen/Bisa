#!/bin/zsh
mkdir -p "$3"

rm -rf "output"

for file in "$2"/*.dat; do
    bin/Debug-macosx/Commissioning/Commissioning "$1" "$file" "output"
    file1="${file##*/}"
    mv output "$3"/"${file1%%.*}"
done