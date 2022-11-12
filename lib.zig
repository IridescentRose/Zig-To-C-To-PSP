extern fn pspDebugScreenInit() void;
extern fn println(str: [*]const u8) void ;

export fn zig_hello() void {
	pspDebugScreenInit();
    println("Hello PSP from Zig!");
}