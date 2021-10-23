
#ifndef SEGMENTINFO_H
#define SEGMENTINFO_H

extern u8 _codeSegmentStart[];         /* code segment start */
extern u8 _codeSegmentBssEnd[];           /* code segment end */

// BGM
extern u8 _seqSegmentRomStart[];
extern u8 _seqSegmentRomEnd[];
extern u8 _midibankSegmentRomStart[];
extern u8 _midibankSegmentRomEnd[];
extern u8 _miditableSegmentRomStart[];
extern u8 _miditableSegmentRomEnd[];

// SFX
extern u8 _sfxbankSegmentRomStart[];
extern u8 _sfxbankSegmentRomEnd[];
extern u8 _sfxtableSegmentRomStart[];
extern u8 _sfxtableSegmentRomEnd[];

// Sprites
extern u8 _hud_iconsSegmentRomStart[];
extern u8 _hud_iconsSegmentRomEnd[];
extern u8 _floor_tilesSegmentRomStart[];
extern u8 _floor_tilesSegmentRomEnd[];
extern u8 _noise_backgroundsSegmentRomStart[];
extern u8 _noise_backgroundsSegmentRomEnd[];
extern u8 _display_textSegmentRomStart[];
extern u8 _display_textSegmentRomEnd[];
extern u8 _level_select_backgroundSegmentRomStart[];
extern u8 _opening_environmentSegmentRomStart[];


// Cutscenes
extern u8 _cutscenebuffersSegmentRomStart[];
extern u8 _packedbackgroundsSegmentRomStart[];

// Dialogue
extern u8 _dialogue_dataSegmentRomStart[];
extern u8 _cast_sprite_dataSegmentRomStart[];
extern u8 _dialogue_backingSegmentRomStart[];

// Maps
extern u8 _map_dataSegmentRomStart[];

#endif /* SEGMENTINFO_H */
