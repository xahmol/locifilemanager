# Loci File Manager
# Written in 2025 by Xander Mol
# 
# https://github.com/xahmol/locifilemanager
# https://www.idreamtin8bits.com/

# For full credits: see src/main.c

# Thanks to Iss for pointers for this Makefile:
# https://forum.defence-force.org/viewtopic.php?p=25411#p25411

## Paths
# Path tp OSDK install. Edit for local dir
# Installation instructions for Linux: #https://forum.defence-force.org/viewtopic.php?p=25396#p25396
OSDK := /home/xahmol/OSDK-build/pc/tools/osdk/main/osdk-linux/bin/
# Emulator path: edit for location of emulator to use
EMUL_DIR := /home/xahmol/oricutron/
# Makefile full path
mkfile_path := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

## Project name
PROJECT      := locifm
PROJECT_DIR  := $(shell pwd)
PROGRAM      := BUILD/$(PROJECT).tap
##DISK_DSK     := BUILD/$(PROJECT).dsk

# Arguments for emulator
# # # Use MACH to override the default and run as:
# # # make run-tap MACH=-ma for Atmos (default)
# # # make run-tap MACH=-m1 for Oric-1
# # # make run-tap MACH=-mo16k for Oric-1 / 16K
# # # make run-tap MACH=-mp for Pravetz-8D
# # # make run-tap MACH=-mt for Telestrat
MACH := -ma
EMU                     := ./oricutron
EMUDIR                  := $(EMUL_DIR)
EMUARG                  := $(MACH)
# # # (un)comment, add, remove more Oricutron cmd-line options
EMUARG                  += --serial none
EMUARG                  += --vsynchack off
EMUARG                  += --turbotape on

## C Sources and library objects to use
SOURCES = src/main.c src/generic.c src/menu.c src/dir.c src/file.c src/drive.c
LSOURCES = $(wildcard libsrc/*.c)
LASOURCES = $(wildcard libsrc/*.s)
LIBRARY = lib/loci.lib
#LIBOBJECTS = 

## CC65 paths and programs
ifndef CC65_HOME
$(warning CC65_HOME not set. Defaulting to $(HOME)/cc65)
export CC65_HOME = $(HOME)/cc65
endif
CC        = $(CC65_HOME)/bin/cl65
AR        = $(CC65_HOME)/bin/ar65
CP        = cp -f

## Compiler and linker flags
CC65_TARGET = atmos
CFLAGS  = -t $(CC65_TARGET) -Oirs --debug-info -I include --asm-include-dir asminc -I asminc
LDFLAGS = -t $(CC65_TARGET) -C $(PROJECT)-cc65.cfg -m $(PROJECT).map

#CC65 has changed internal library prefix for errno related symbols. Check which one we have.
CHECK_CC65 != CC65_HOME=$(CC65_HOME) $(CC) -c $(CFLAGS) -o .asm_check libsrc/mia.s 2>&1

########################################

.SUFFIXES:
.PHONY: all clean run run-debug
all: $(PROGRAM)

ifneq ($(MAKECMDGOALS),clean)
-include $(SOURCES:.c=.d) $(SOURCESUPD:.c=.d)
endif

# Compile C sources
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.s
ifneq ($(strip $(CHECK_CC65)),)
	$(CC) -c $(CFLAGS) --asm-define OLD_CC65 -o $@ $<
else
	$(CC) -c $(CFLAGS) -o $@ $<
endif

# Buold librarry
$(LIBRARY): $(LSOURCES:.c=.o) $(LASOURCES:.s=.o)
	$(CP) $(CC65_HOME)/lib/$(CC65_TARGET).lib $(LIBRARY)
	$(AR) a $(LIBRARY) $(LSOURCES:.c=.o) $(LASOURCES:.s=.o)

# Link compiled objects 
$(PROGRAM): $(SOURCES:.c=.o) $(LIBRARY)
	$(CC) $(LDFLAGS) -o  $@ $^ src/tapehdr.s 

# Build disk
#$(DISK_DSK): $(PROGRAM) $(TESTPROG_BIN)
#	$(OSDK)header $(PROGRAM) BUILD/$(PROJECT).tap 0x0501
#	$(OSDK)tap2dsk -iCLS:$(PROJECT) -c20:3 -n$(PROJECT) BUILD/$(PROJECT).tap OSEHS1.tap OSEHS2.tap OSEHS3.tap OSEHS4.tap OSETSC.tap PETSCIIPJ.tap PETSCIISC.tap PETSCIICS.tap PETSCIICA.tap $(DISK_DSK)
#	$(OSDK)old2mfm $(DISK_DSK)
#	cd $(HXCFE); ./hxcfe -finput:"$(mkfile_path)/$(DISK_DSK)" -foutput:"$(mkfile_path)/$(DISK_HFE)" -conv:HXC_HFE

# Clean old builds and objects
clean:
	$(RM) $(SOURCES:.c=.o) $(SOURCES:.c=.d) $(LSOURCES:.c=.o) $(LASOURCES:.s=.o) *.map *.brk *.sym .asm_check
#	cd BUILD; $(RM) *.*

# Execute in emulator: use make run
run: $(PROGRAM)
	cd $(EMUDIR); $(EMU) $(EMUARG) "$(PROJECT_DIR)/$(PROGRAM)"