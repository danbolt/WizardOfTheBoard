#!/bin/bash

./convert.sh
./compress.sh
./generatebankinfo.sh > sfx.ins
./createbank.sh
./generateheader.sh > sfx.h

