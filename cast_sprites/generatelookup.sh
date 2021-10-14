#!/bin/bash

gperf --no-strlen --lookup-function-name=getCastTextureOffset --struct-type texture-gperf-mapping --output-file=castlookup.c 