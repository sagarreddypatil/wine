# Implementation Context

## What was done

Replaced Wine's macOS OpenGL backend (`dlls/winemac.drv/opengl.c`) from the old CGL-based
implementation (~2600 lines using Apple's deprecated OpenGL) with a minimal EGL-based one (~180 lines)
that routes OpenGL through Mesa's Zink driver via the surfaceless EGL platform.

### Architecture

```
Windows OpenGL app (WGL) → Wine win32u/opengl.c (WGL→EGL translation)
  → winemac.drv/opengl.c (EGL surface creation with CAMetalLayer)
  → Mesa EGL surfaceless platform → Zink (GL→Vulkan) → KosmicKrisp (Vulkan→Metal) → GPU
```

### Key files modified

- **`dlls/winemac.drv/opengl.c`** — Complete rewrite. Uses EGL surfaceless platform. The
  `macdrv_surface_create` function creates a `WineMetalView` with a proper `CAMetalLayer` (same
  approach as `vulkan.c`) and passes it to `eglCreateWindowSurface`. Delegates pixel format
  enumeration, context creation, and pbuffer handling to win32u's default EGL implementations.

- **`dlls/winemac.drv/cocoa_window.m`** — Added `macdrv_view_get_backing_layer()` function that
  returns a cocoa view's backing `CALayer` (not currently used after the CAMetalLayer fix, but kept
  for potential future use).

- **`dlls/winemac.drv/macdrv_cocoa.h`** — Added declaration for `macdrv_view_get_backing_layer`.

### Key design decisions

1. **CAMetalLayer vs plain CALayer**: Initially passed the `WineContentView`'s plain `CALayer` to
   `eglCreateWindowSurface` — this resulted in Zink initializing but no visible window. The fix was
   to create a `WineMetalView` + `CAMetalLayer` (mirroring `vulkan.c`'s approach) and pass that
   instead. The `macdrv_client_surface` struct already had `metal_device`/`metal_view` fields and
   cleanup code from the Vulkan path.

2. **EGL platform**: Uses `EGL_PLATFORM_SURFACELESS_MESA` with `native_display = 0` and
   `force_pbuffer_formats = TRUE` (same as Wayland driver).

3. **Delegated functions**: Context creation, pixel format enumeration, pbuffer handling, etc. are
   all copied from the default egldrv implementations in `win32u/opengl.c` during `macdrv_OpenGLInit`.

### Reference: Wayland driver

The implementation is modeled after `dlls/winewayland.drv/opengl.c` which is the closest existing
EGL-based backend. Key differences:
- Wayland uses `EGL_PLATFORM_WAYLAND_KHR` + `wl_egl_window`; we use surfaceless + `CAMetalLayer`
- Wayland has custom pbuffer handling (dummy wl_surface); we use the default egldrv pbuffer
- Wayland syncs drawable size on flush; we don't currently (may need to add for resize support)

## Current status

- **glxgears**: WORKS
- **GLSLTestbed**: Window opens (black), shader compilation fails on `any(bool)` (Mesa GLSL
  limitation — `any()` only accepts bvec types). After shader error, deadlock in
  `vectored_handlers_section` (not investigated, user deferred).

## Build notes

- No reconfigure needed — just `arch -x86_64 make -j$(sysctl -n hw.ncpu)` + `arch -x86_64 make install`
- `SONAME_LIBEGL` is confirmed set, don't worry about it
- Mesa installed at `~/mesa-native` (x86-64), Wine at `~/wine-custom`
- test.sh sets: `EGL_PLATFORM=surfaceless`, `MESA_LOADER_DRIVER_OVERRIDE=zink`,
  `MESA_GL_VERSION_OVERRIDE=4.6`, `VK_DRIVER_FILES=...kosmickrisp...`
