# export DYLD_INSERT_LIBRARIES=$HOME/mesa-native/lib/libgl_interpose.dylib
# export DYLD_LIBRARY_PATH=$HOME/mesa-native/lib
export LIBGL_DRIVERS_PATH=$HOME/mesa-native/lib/dri
export VK_DRIVER_FILES=$HOME/mesa-native/share/vulkan/icd.d/kosmickrisp_mesa_icd.x86_64.json
export EGL_PLATFORM=surfaceless
export MESA_LOADER_DRIVER_OVERRIDE=zink
export MESA_GL_VERSION_OVERRIDE=4.6
export MESA_GLSL_VERSION_OVERRIDE=460

cd ~/Downloads/
# ~/wine-custom/bin/wine glxgears.x64.exe
~/wine-custom/bin/wine gl46test.exe
# cd ~/Downloads/GLSLTestbed-Release
# ~/wine-custom/bin/wine GLSLTestbed.exe
