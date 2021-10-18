/*
	ROM spec file

	Main memory map

  	0x80000000  exception vectors, ...
  	0x80000400  zbuffer (size 320*240*2)
  	0x80025c00  codesegment
	      :  
  	0x8038F800  cfb 16b 3buffer (size 320*240*2*3)

	Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

OUTPUT_ARCH (mips)

#include <nusys.h>

#define BEGIN_SEG(name, addr) \
    _##name##SegmentStart = ADDR(.name); \
    _##name##SegmentRomStart = __romPos; \
    .name addr : AT(__romPos)

#define END_SEG(name) \
    _##name##SegmentEnd = ADDR(.name) + SIZEOF(.name); \
    _##name##SegmentRomEnd = __romPos + SIZEOF(.name); \
    __romPos += SIZEOF(.name);

#define BEGIN_NOLOAD(name) \
    _##name##SegmentBssStart = ADDR(.name.noload); \
    .name.noload (NOLOAD) :

#define END_NOLOAD(name) \
    _##name##SegmentBssEnd = ADDR(.name.noload) + SIZEOF(.name.noload); \
    _##name##SegmentBssSize = SIZEOF(.name.noload);

SECTIONS
{
   __romPos = 0;

   BEGIN_SEG(boot, 0x04000000)
   {
      asm/rom_header.o(.text);
      boot.6102.o(.data);
   }
   END_SEG(boot)

   BEGIN_SEG(code, NU_SPEC_BOOT_ADDR) SUBALIGN(16)
   {
      /* text */
      asm/entry.o(.text);
      codesegment.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/aspMain.o(.text);
      /usr/lib/n64/PR/n_aspMain.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.text);

      /* data */
      codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/aspMain.o(.data*);
      /usr/lib/n64/PR/n_aspMain.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.data*);

      /* rodata */
      codesegment.o(.rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      codesegment.o(COMMON);
      codesegment.o(.scommon*);
      codesegment.o(.bss*);
      . = ALIGN(0x8);
   }
   END_NOLOAD(code)

   BEGIN_SEG(sound_data, __romPos)
   {
      sound_data.o(.data);
   }
   END_SEG(sound_data)

   BEGIN_SEG(hud_icons, __romPos)
   {
      sprites/hud_icons.bino(.data);
   }
   END_SEG(hud_icons)

   BEGIN_SEG(floor_tiles, __romPos)
   {
      sprites/floor_tiles.bino(.data);
   }
   END_SEG(floor_tiles)

   BEGIN_SEG(noise_backgrounds, __romPos)
   {
      sprites/noise_backgrounds.bino(.data);
   }
   END_SEG(noise_backgrounds)

   BEGIN_SEG(display_text, __romPos)
   {
      sprites/display_text.bino(.data);
   }
   END_SEG(display_text)

   BEGIN_SEG(dialogue_data, __romPos)
   {
      dialogue/dialogueBuffers.bino(.data);
   }
   END_SEG(dialogue_data)

   BEGIN_SEG(cast_sprite_data, __romPos)
   {
      cast_sprites/packedtextures.bino(.data);
   }
   END_SEG(cast_sprite_data)

   BEGIN_SEG(map_data, __romPos)
   {
      maps/mapbuffers.bino(.data);
   }
   END_SEG(map_data)

   BEGIN_SEG(level_select_background, __romPos)
   {
      sprites/level_select_background.bino(.data);
   }
   END_SEG(level_select_background)

   BEGIN_SEG(packedbackgrounds, __romPos)
   {
      cutscene_backgrounds/packedbackgrounds.bino(.data);
   }
   END_SEG(packedbackgrounds)
   
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
