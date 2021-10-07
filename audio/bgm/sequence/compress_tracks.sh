rm -rf compressed
mkdir -p compressed
for i in converted/*.mid; do
    o=compressed/${i#converted/}
    midicomp -o $i $o
done
