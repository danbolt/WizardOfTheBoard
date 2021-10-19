#!/bin/bash

node processcutscenes.js

gperf --no-strlen --lookup-function-name=getCutsceneOffset --struct-type cutscene-gperf-mapping --output-file=cutscenelookup.c 