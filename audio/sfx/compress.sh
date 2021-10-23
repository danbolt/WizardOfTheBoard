#!/bin/bash
mkdir -p table
for i in converted/*.aiff; do
    o=table/${i#converted/}
    wine ../sgi2pc/tabledesign.exe "$i" > "${o%.aiff}.table"
done

mkdir -p compressed
for i in table/*.table; do
    o=compressed/${i#table/}
    inp=converted/${i#table/}
    wine ../sgi2pc/ADPCMENC.EXE -c "$i" "${inp%.table}.aiff"  "${o%.table}.aifc"
done

