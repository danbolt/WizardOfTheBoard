#!/bin/bash

rm -rf processed
mkdir -p processed
node mapprocess.js


rm -rf generated
mkdir -p generated
node mapgenerate.js

gperf --no-strlen --lookup-function-name=getMapDataOffset --struct-type map-gperf-mapping --output-file=maplookup.c 