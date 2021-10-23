#!/bin/bash

count=10
sounds=()
for i in compressed/*.aifc; do
  nm=${i#compressed/}
  name=${nm%.aifc}

  durationInSeconds=$(sox original/${name}.wav -n stat 2>&1 | sed -n 's#^Length (seconds):[^0-9]*\([0-9.]*\)$#\1#p')
  durationMicroseconds=$(echo "($durationInSeconds * 1000000)/1" | bc)

  echo "envelope FX_${name}_env"
  echo "{"
  echo "    attackTime    = 0;"
  echo "    attackVolume  = 100;"
  echo "    decayTime   = ${durationMicroseconds};"
  echo "    decayVolume   = 90;"
  echo "    releaseTime   = 0;"
  echo "    "
  echo "}"

  sounds+=(${name})

  echo "keymap FX_${name}_keymap"
  echo "{"
  echo "    velocityMin = 0;"
  echo "    velocityMax = 127;"
  echo "    keyMin      = ${count};"
  echo "    keyMax      = ${count};"
  echo "    keyBase     = ${count};"
  echo "    detune      = 0;"
  echo "}"

  echo ""

  echo "sound FX_${name}Sound"
  echo "{"
  echo "    use (\"compressed/${name}.aifc\");"
  echo "    "

  echo "    pan    = 64;"
  echo "    volume = 127;"
  echo "    keymap = FX_${name}_keymap;"
  echo "    envelope = FX_${name}_env;"
  echo "}"

  count=$((count+1))
done

echo ""

echo "instrument sfx"
echo "{"
echo "    volume = 127;"
echo "    pan    = 64;"
echo "    "

for var in "${sounds[@]}"
do
  echo "    sound = FX_${var}Sound;"
  # do something on $var
done

echo "}"

echo ""



echo "bank sfxbank"
echo "{"
echo "    sampleRate = 32000;"
echo "    "
echo "    instrument [0] = sfx;"
echo "}"

echo ""
