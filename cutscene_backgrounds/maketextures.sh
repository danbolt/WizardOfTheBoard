#!/bin/bash

rm -fr generated
mkdir -p generated
for sourcePath in source/*.png; do
    generatedPathPNG=generated/${sourcePath#source/}
    generatedPathBIN=${generatedPathPNG/png/bin}
    n64graphics -i $generatedPathBIN -g $sourcePath -f rgba16 -w 320 -h 240
done