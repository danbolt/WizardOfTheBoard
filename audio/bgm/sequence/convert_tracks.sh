rm -rf converted
mkdir -p converted
for i in source/*.mid; do
    o=converted/${i#source/}
    midicvt $i $o
done
