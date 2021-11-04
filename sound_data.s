.include "macros.inc"

.section .data

#ifdef NO_COMPILED_AUDIO

glabel _miditableSegmentRomStart
glabel _miditableSegmentRomEnd

glabel _midibankSegmentRomStart
glabel _midibankSegmentRomEnd

glabel _seqSegmentRomStart
glabel _seqSegmentRomEnd

glabel _sfxbankSegmentRomStart
glabel _sfxbankSegmentRomEnd

glabel _sfxtableSegmentRomStart
glabel _sfxtableSegmentRomEnd

#else

glabel _miditableSegmentRomStart
.incbin "audio/bgm/bank/DrumsOnGunshot.tbl"
.balign 16
glabel _miditableSegmentRomEnd

glabel _midibankSegmentRomStart
.incbin "audio/bgm/bank/DrumsOnGunshot.ctl"
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

#endif