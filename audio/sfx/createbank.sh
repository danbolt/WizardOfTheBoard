#!/bin/bash

./generatebankinfo.sh > sfx.ins

wine ../sgi2pc/ic.exe sfx.ins |  iconv -f SHIFT-JIS -t UTF-8
echo ""
