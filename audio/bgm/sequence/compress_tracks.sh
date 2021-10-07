rm -rf compressed
mkdir -p compressed
for i in converted/*.mid; do
    o=compressed/${i#converted/}
    wine ../../sgi2pc/midicomp.exe -o $i $o
done
