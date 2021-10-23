#!/bin/bash
mkdir -p table
for i in converted/*.aiff; do
    o=table/${i#converted/}
    tabledesign "$i" > "${o%.aiff}.table"
done

mkdir -p compressed
for i in table/*.table; do
    o=compressed/${i#table/}
    inp=converted/${i#table/}
    vadpcm_enc -c "$i" "${inp%.table}.aiff"  "${o%.table}.aifc"
done

