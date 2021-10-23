#!/bin/bash

./generatebankinfo.sh > sfx.ins

ic -o sfx sfx.ins
echo ""
