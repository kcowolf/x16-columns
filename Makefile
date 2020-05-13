PRG= COLUMNS.PRG

SRC= src
INC= inc
LIB= submodules/libX16/lib
LIBINC= submodules/libX16/include
GFX= gfx

RM= rm
CP= cp
CC= cc65
CA= ca65
LD= ld65
ECHO= echo
MKDIR= mkdir

SRC_C= $(wildcard $(SRC)/*.c)
SRC_C += $(wildcard $(SRC)/GAME/*.c)
SRC_C += $(wildcard $(SRC)/GFX/*.c)
SRC_BIN= $(wildcard $(GFX)/*.BIN)

INCS= -I$(INC) -I$(SRC) -I$(LIBINC)

OBJ= $(SRC_C:.c=.o)
OBJS= $(addprefix out/, $(OBJ))

BINS= $(addprefix out/, $(SRC_BIN))
OUTBINS= $(addprefix out/, $(notdir ${SRC_BIN}))

LIBX16= $(LIB)/libX16.lib

release: pre-build out/$(PRG)

all: release
default: release

clean: cleanobj cleanbin
	$(RM) -f out/$(PRG)

cleanbin:
	$(RM) -f $(OUTBINS)

cleanobj:
	$(RM) -f $(OBJS)

pre-build:
	$(MKDIR) -p out
	$(MKDIR) -p out/src
	$(MKDIR) -p out/src/GAME
	$(MKDIR) -p out/src/GFX

out/%.BIN: %.BIN
	$(CP) $< out/$(notdir $@)

out/%.o: out/%.s
	$(CA) -t cx16 $<

out/%.s: %.c
	$(CC) -t cx16 $(INCS) -Oi -o out/$(<:%.c=%.s) $<

out/%.PRG: $(OBJS) $(LIBX16) $(BINS)
	$(LD) -t cx16 -o $@ $(OBJS) $(LIBX16) cx16.lib
