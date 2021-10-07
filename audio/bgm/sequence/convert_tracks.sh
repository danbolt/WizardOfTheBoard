rm -rf converted
mkdir -p converted
for i in source/*.mid; do
    o=converted/${i#source/}
    wine ../../sgi2pc/MIDICVT.EXE $i $o
done
