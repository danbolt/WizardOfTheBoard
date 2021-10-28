.include "macros.inc"

.section .data

glabel _miditableSegmentRomStart
.incbin "audio/bgm/bank/GenMidiBank.tbl"
.balign 16
glabel _miditableSegmentRomEnd

glabel _midibankSegmentRomStart
.incbin "audio/bgm/bank/GenMidiBank.ctl"
.balign 16
glabel _midibankSegmentRomEnd

glabel _seqSegmentRomStart
.incbin "audio/bgm/sequence/sequences.sbk"
.balign 16
glabel _seqSegmentRomEnd

glabel _sfxbankSegmentRomStart
.incbin "audio/sfx/sfx.ctl"
.balign 16
glabel _sfxbankSegmentRomEnd

glabel _sfxtableSegmentRomStart
.incbin "audio/sfx/sfx.tbl"
.balign 16
glabel _sfxtableSegmentRomEnd
