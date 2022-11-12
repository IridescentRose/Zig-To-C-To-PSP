# Zig-To-C-To-PSP
Running Zig on PSP using the C backend.

To build, have PSP Toolchain installed and Zig.
Run: `make psp`
Put the EBOOT on your PSP, and you're all set.

lib.zig:
```zig
extern fn pspDebugScreenInit() void;
extern fn println(str: [*]const u8) void ;

export fn zig_hello() void {
	pspDebugScreenInit();
    println("Hello World!");
}
```

main.c:
```c
#include <pspkernel.h>
#include <pspdebug.h>
#include <stdbool.h>
#include "lib.h" 

PSP_MODULE_INFO("Tutorial", 0, 1, 0);

void println(const unsigned char* str) {
    pspDebugScreenPrintf("%s\n", str);
}
 
int main() { //In C++ `auto main() -> int` is also valid.
	SetupCallbacks();

    zig_hello();

    while(true);
	return 0;
}
```

Makefile:
```
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
```