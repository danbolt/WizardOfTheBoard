# 	Based off the nu1 modernsdk implentation

include /usr/include/n64/make/PRdefs

# The directory which has the include file and library of NuSYSTEM
#
NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

LIB = /usr/lib/n64
LPR = $(LIB)/PR
INC = /usr/include/n64

N_AUDIO = yes

ifdef N_AUDIO
NUAUDIOLIB = -lnualsgi_n -ln_audio
else
NUAUDIOLIB = -lnualsgi
endif

LCDEFS =	-DNDEBUG -DF3DEX_GBI_2
LCINCS =	-I. -I$(NUSYSINCDIR) -I/usr/include/n64/PR -I/usr/include/n64/nustd
LCOPTS =	-G 0 
LDFLAGS = $(MKDEPOPT) -L$(LIB) -L$(NUSYSLIBDIR) $(NUAUDIOLIB) -lnusys -lnustd -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =  $(ELF) $(ASMOBJECTS) $(CP_LD_SCRIPT) $(MAP) $(TARGETS)

OPTIMIZER =	-g

ELF		= jam2.elf
TARGETS	= jam2.z64
MAP		= jam2.map
LD_SCRIPT	= jam2.ld
CP_LD_SCRIPT	= jam2_cp.ld

HFILES =	main.h nustdfuncs.h stagekeys.h graphic.h audio/sfx/sfx.h splashscreen.h jinglescreen.h gameaudio.h backgroundbuffers.h levelselect.h stage00.h credits.h displaytext.h cutscene.h titlescreen.h sixtwelve.h sixtwelve_helpers.h constants.h gamemath.h dialogue.h segmentinfo.h audio/bgm/sequence/tracknumbers.h pieces.h board.h monsters.h betweenstages.h mapdata.h dialogue/dialoguelookup.h cast_sprites/castlookup.h map/maplookup.h cutscene_backgrounds/backgroundlookup.h cutscenes/cutscenelookup.h opening/envtexture.h opening/tower.h opening/ground.h bip-mapping/bipmapping.h

ifdef NO_COMPILED_AUDIO
LCDEFS += -DNO_COMPILED_AUDIO
N64_ASFLAGS += -DNO_COMPILED_AUDIO
else
endif

ifdef PAL_ROM
LCDEFS += -DPAL_ROM
TARGETS	= jam2_pal.z64
endif

ASMFILES	= asm/entry.s asm/rom_header.s sound_data.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODEFILES   = 	main.c nustdfuncs.c backgroundbuffers.c splashscreen.c jinglescreen.c gameaudio.c stagekeys.c stage00.c levelselect.c displaytext.c titlescreen.c credits.c cutscene.c graphic.c sixtwelve.c sixtwelve_tex.c sixtwelve_helpers.c gfxinit.c gamemath.c dialogue.c pieces.c pawn.c rook.c bishop.c queen.c knight.c king.c betweenstages.c wall.c board.c cursor.c toad.c projectile.c shadowqueen.c ogre.c jumper.c snake.c dialogue/dialoguelookup.c cast_sprites/castlookup.c maps/maplookup.c cutscene_backgrounds/backgroundlookup.c cutscenes/cutscenelookup.c opening/envtexture.c bip-mapping/bipmapping.c

ifdef USB_DEBUGGING
CODEFILES += usb.c debug.c
HFILES += usb.h debug.h
LCDEFS += -DGAME_USB_DEBUGGING
endif

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys_rom.o

DATAFILES   =	

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJECTS)

RAWDATAOBJ = sprites/hud_icons.bino sprites/level_select_icons.bino sprites/zatt_potraits.bino sprites/floor_tiles.bino sprites/floor_tiles2.bino sprites/stairs_anim.bino sprites/dialogue_backing.bino sprites/idea.bino sprites/noise_backgrounds.bino sprites/display_text.bino dialogue/dialogueBuffers.bino cast_sprites/packedtextures.bino maps/mapbuffers.bino sprites/level_select_background.bino cutscene_backgrounds/packedbackgrounds.bino cutscenes/cutscenebuffers.bino

default:        $(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

.s.o:
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

sprites/%.bino: sprites/%.bin
	mips-n64-objcopy  -I binary -B mips -O elf32-bigmips $< $@

cutscene_backgrounds/%.bino: cutscene_backgrounds/%.bin
	mips-n64-objcopy  -I binary -B mips -O elf32-bigmips $< $@

dialogue/%.bino: dialogue/%.bin
	mips-n64-objcopy  -I binary -B mips -O elf32-bigmips $< $@

maps/%.bino: maps/%.bin
	mips-n64-objcopy  -I binary -B mips -O elf32-bigmips $< $@

cast_sprites/%.bino: cast_sprites/%.bin
	mips-n64-objcopy  -I binary -B mips -O elf32-bigmips $< $@

cutscenes/%.bino: cutscenes/%.bin
	mips-n64-objcopy  -I binary -B mips -O elf32-bigmips $< $@


$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINCDIR) -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT) $(RAWDATAOBJ)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
