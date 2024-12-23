#
# This is the YAST Makefile
# Date: 2024-10-30
#

VERSION := 0.5.4

# GPIOLIB := -DWIRINGPI
GPIOLIB := -DLGPIO

# HARDWARE := -DSN3218=1
# HARDWARE := -DMCP23017=1
# HARDWARE := -DMCP23017EASY=1
# HARDWARE := -DMCP23008=1

# FONTSIZE := -DFONT3X3
FONTSIZE := -DFONT3X5

# ALSA_SOUND_LIB := -lasound
# ALSA_SOUND := -DALSA_SOUND

# ------- don't edit from here --------

CC	:= gcc

# CP437 version
CFLAGS  := -Wall $(ALSA_SOUND) $(HARDWARE) -DVERSION=\"$(VERSION)\" $(FONTSIZE) $(GPIOLIB)
# UNICODE version
# CFLAGS  := -Wall -lncursesw -lm -lwiringPi $(ALSA_SOUND_LIB) $(ALSA_SOUND) $(HARDWARE) -DVERSION=\"$(VERSION)\" $(FONTSIZE) -DUNICODE

# LDLIBS  = -lwiringPi -lncurses -lm $(ALSA_SOUND_LIB)
LDLIBS  = -llgpio -lncurses -lm $(ALSA_SOUND_LIB)

# MYFLAGS := -DALSA_SOUND -DMCP23017=1 -lasound 
MYFLAGS := -DALSA_SOUND -lasound -DI2C
SRCDIR  := src/
OBJDIR  := obj/
DATADIR := result/
DOCS := docs/
OUTNAME := yast

C_FILES := $(wildcard $(SRCDIR)*.c)
OBJ_FILES := $(addprefix $(OBJDIR),$(notdir $(C_FILES:.c=.o)))

yast:	$(OBJ_FILES)
	$(CC)  -o $(OUTNAME) $^ $(CFLAGS) $(LDLIBS)

test: CFLAGS += $(MYFLAGS)

test: 	all

all: 	yast 

$(OBJDIR)%.o: $(SRCDIR)%.c 
	$(CC) $(CFLAGS) -c -o $@ $<

clean: 
	rm -f *.bak *~ *.exe *.o *.log block
	rm -f $(SRCDIR)*.c~
	rm -f $(SRCDIR)*.h~
	rm -f $(SRCDIR)*.o
	rm -f $(SRCDIR)*.bak
	rm -f $(OBJDIR)*.o
	rm -f script/*~
	rm -f $(DOCS)*~
	rm -f $(OUTNAME)
	rm -f $(DATADIR)timing_*
	rm -f yastdata.txt
	
rasptar: 
	tar -cvzf yast_$(VERSION).tgz src script obj result docs Makefile README.md hardware.md install.md LICENSE yast.conf

