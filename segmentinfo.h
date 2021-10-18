
#ifndef SEGMENTINFO_H
#define SEGMENTINFO_H

extern char _codeSegmentStart[];         /* code segment start */
extern char _codeSegmentBssEnd[];           /* code segment end */

extern u8 _seqSegmentRomStart[];
extern u8 _seqSegmentRomEnd[];
extern u8 _midibankSegmentRomStart[];
extern u8 _midibankSegmentRomEnd[];
extern u8 _miditableSegmentRomStart[];
extern u8 _miditableSegmentRomEnd[];

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

// Dialogue
extern u8 _dialogue_dataSegmentRomStart[];
extern u8 _cast_sprite_dataSegmentRomStart[];

// Maps
extern u8 _map_dataSegmentRomStart[];

#endif /* SEGMENTINFO_H */
