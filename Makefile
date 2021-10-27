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
LCINCS =	-I. -I$(NUSYSINCDIR) -I/usr/include/n64/PR
LCOPTS =	-G 0
LDFLAGS = $(MKDEPOPT) -L$(LIB) -L$(NUSYSLIBDIR) $(NUAUDIOLIB) -lnusys -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =  $(ELF) $(ASMOBJECTS) $(CP_LD_SCRIPT) $(MAP) $(TARGETS)

OPTIMIZER =	-g

ELF		= jam2.elf
TARGETS	= jam2.z64
MAP		= jam2.map
LD_SCRIPT	= jam2.ld
CP_LD_SCRIPT	= jam2_cp.ld

HFILES =	main.h nustdfuncs.h stagekeys.h graphic.h audio/sfx/sfx.h backgroundbuffers.h levelselect.h displaytext.h cutscene.h titlescreen.h sixtwelve.h sixtwelve_helpers.h constants.h gamemath.h dialogue.h segmentinfo.h audio/bgm/sequence/tracknumbers.h pieces.h board.h monsters.h mapdata.h dialogue/dialoguelookup.h cast_sprites/castlookup.h map/maplookup.h cutscene_backgrounds/backgroundlookup.h cutscenes/cutscenelookup.h opening/envtexture.h opening/tower.h opening/ground.h bip-mapping/bipmapping.h

ASMFILES	= asm/entry.s asm/rom_header.s sound_data.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODEFILES   = 	main.c nustdfuncs.c backgroundbuffers.c stagekeys.c stage00.c levelselect.c displaytext.c titlescreen.c cutscene.c graphic.c sixtwelve.c sixtwelve_tex.c sixtwelve_helpers.c gfxinit.c gamemath.c dialogue.c pieces.c pawn.c rook.c bishop.c queen.c knight.c king.c wall.c board.c cursor.c toad.c projectile.c ogre.c snake.c dialogue/dialoguelookup.c cast_sprites/castlookup.c maps/maplookup.c cutscene_backgrounds/backgroundlookup.c cutscenes/cutscenelookup.c opening/envtexture.c bip-mapping/bipmapping.c

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys_rom.o

DATAFILES   =	

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJECTS)

RAWDATAOBJ = sprites/hud_icons.bino sprites/level_select_icons.bino sprites/zatt_potraits.bino sprites/floor_tiles.bino sprites/dialogue_backing.bino sprites/idea.bino sprites/noise_backgrounds.bino sprites/display_text.bino dialogue/dialogueBuffers.bino cast_sprites/packedtextures.bino maps/mapbuffers.bino sprites/level_select_background.bino cutscene_backgrounds/packedbackgrounds.bino cutscenes/cutscenebuffers.bino

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
	$(OBJCOPY) --pad-to=0x200000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
