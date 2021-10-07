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

OPTIMIZER =	-O2

ELF		= jam2.elf
TARGETS	= jam2.z64
MAP		= jam2.map
LD_SCRIPT	= jam2.ld
CP_LD_SCRIPT	= jam2_cp.ld

HFILES =	main.h graphic.h segmentinfo.h

ASMFILES	= asm/entry.s asm/rom_header.s sound_data.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODEFILES   = 	main.c stage00.c graphic.c gfxinit.c

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys_rom.o

DATAFILES   =	

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJECTS)

default:        $(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINCDIR) -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
