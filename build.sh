#!/bin/bash
set -euo pipefail

export PATH="/usr/local/opt/bison/bin:/usr/local/bin:$PATH"
export CPPFLAGS="-I$HOME/mesa-native/include"
export LDFLAGS="-L/usr/local/lib -L$HOME/mesa-native/lib -Wl,-rpath,/usr/local/lib -Wl,-rpath,$HOME/mesa-native/lib"
export PKG_CONFIG_PATH="$HOME/mesa-native/lib/pkgconfig"
rm -f config.cache

arch -x86_64 make distclean || true

arch -x86_64 ./configure \
  --prefix="$HOME/wine-custom" \
  --enable-win64 \
  --with-opengl \
  --without-vulkan \
  --without-x

arch -x86_64 make -j"$(sysctl -n hw.ncpu)"
arch -x86_64 make install
