yabai --stop-service
PREFIX=/opt/wine-gl46
# export DYLD_INSERT_LIBRARIES=$PREFIX/lib/libgl_interpose.dylib
export DYLD_LIBRARY_PATH=$PREFIX/lib
export LIBGL_DRIVERS_PATH=$PREFIX/lib/dri
export VK_DRIVER_FILES=$PREFIX/share/vulkan/icd.d/kosmickrisp_mesa_icd.x86_64.json
export EGL_PLATFORM=surfaceless
export MESA_LOADER_DRIVER_OVERRIDE=zink
export MESA_GL_VERSION_OVERRIDE=4.6
export MESA_GLSL_VERSION_OVERRIDE=460

WINE=$PREFIX/bin/wine

cd ~/Downloads/
$WINE gl46test.exe
# $WINE glxgears.x64.exe
# cd ~/Downloads/GLSLTestbed-Release
# $WINE GLSLTestbed.exe
yabai --start-service