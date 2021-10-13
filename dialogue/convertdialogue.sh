#!/bin/bash

# todo: process each file

rm -rf processed
mkdir -p processed
for sourcePath in source/*.dialogue; do
    node process.js $sourcePath
done


node pack.js

gperf --no-strlen --lookup-function-name=getDialogueDataOffset --struct-type map-gperf-mapping --output-file=dialoguelookup.c 