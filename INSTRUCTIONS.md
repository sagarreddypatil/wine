# Instructions

I am trying to run Windows OpenGL 4.6 (specifically OpenGL 4.6, not DirectX) applications on macOS.

This is partially based off of [this gist](https://gist.github.com/lucamignatti/5312f5e937de2ba44256ecba6de54cc2)
where someone was able to run minecraft with OpenGL 4.6 by making modifications to mesa and GLFW.

Please read the gist in its entirety.

The modifications to mesa allow running OpenGL through KosmicKrisp Vulkan using Zink. The patches
to GLFW are pretty minimal and it's just to remove the Apple special cases and use EGL directly.

I have compiled this mesa (for x86-64, because wine doesn't support a native arm64 build at the
moment), and the install prefix is located at `~/mesa-native`. Do not fuck with these files.
They have been tested to work with the modified GLFW to run minecraft and work flawlessly.

Here's what you need to do - currently Wine's OpenGL implementation for winemac.drv is pretty
extensive, and you'll need to modify that to be basically a copy of an existing OpenGL backend
(probably the Wayland one) which uses EGL. The main file you'll be working in is
`dlls/winemac.drv/opengl.c`. For EGL specifically, a lot of the logic is already located in
`dlls/win32u/opengl.c` and the backends for Wayland/X11/etc. are fairly minimal. Note that it's ok
for Wine to create a `NSViewBackingLayer`, since the modified mesa will convert that to
`CAMetalLayer` as mentioned in the gist.

To test your changes. you build with `./build.sh` and test with `test.sh` (you shouldn't need to
make any modifications to these scripts).

`build.sh` will build and install it to `~/wine-custom/`. `test.sh` will then run that version on
https://github.com/konstatoivanen/GLSLTestbed which is a windows binary which tests OpenGL 4.6
functionality. There is currently already a compiled version in `~/wine-custom/` which when you run
`./test.sh` fails with 
```
λ ./test.sh
00e0:err:vulkan:vulkan_init_once Wine was built without Vulkan support.
GLFW Initialized
Creating window PK Renderer (1024, 512)
0024:fixme:msg:ChangeWindowMessageFilterEx 000000000001005E 233 1 0000000000000000
0024:fixme:msg:ChangeWindowMessageFilterEx 000000000001005E 4a 1 0000000000000000
0024:fixme:msg:ChangeWindowMessageFilterEx 000000000001005E 49 1 0000000000000000
Failed To Create Window
```

And has to be `Ctrl-C`'d to exit.

NOTE: Wine was built without Vulkan support error is a red herring and can safely be ignored. The
thing using Vulkan is Mesa, not Wine.

Your goal is to get this test to work.

## Addendum

Instead of running `./test.sh`, ask me (the user) to run it, and paste the output to OUTPUT.md
please. When you run it yourself, you are unable to potentially view/interact with the target
application and unable to recognize success/failure/hang and properly close the application.

## Prior work

See `CONTEXT.md` for full implementation context, architecture, key design decisions, and current
status. Read it before continuing any work.