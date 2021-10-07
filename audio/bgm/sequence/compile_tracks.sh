
tracks=$(ls compressed/*)

sbc -o sequences.sbk $tracks
#wine ../../sgi2pc/SBC.EXE -Osequences.sbk compressed/test_track_a.mid