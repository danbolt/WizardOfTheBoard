#!/bin/bash

gperf --no-strlen --lookup-function-name=getBackgroundTextureOffset --struct-type texture-gperf-mapping --output-file=backgroundlookup.c 