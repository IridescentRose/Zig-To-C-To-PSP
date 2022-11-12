TARGET = ZTest
OBJS = main.o lib.o

INCDIR = $(PSPPATH)/include
INCDIR += .
CFLAGS = 
CXXFLAGS = $(CFLAGS) -std=c++14 -std=c11 -fno-rtti
ASFLAGS = $(CFLAGS)
 
# PSP Stuff
BUILD_PRX = 1
PSP_FW_VERSION = 660
PSP_LARGE_MEMORY = 1
 
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Zig Test
#PSP_EBOOT_ICON = ICON0.PNG 
 
zig: 
	zig build-obj -ofmt=c lib.zig -femit-h

psp: zig all

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak