#!/bin/bash
set -euo pipefail

PREFIX=/opt/wine-gl46
export PATH="/usr/local/opt/bison/bin:/usr/local/bin:$PATH"
export CPPFLAGS="-I$PREFIX/include"
export LDFLAGS="-L/usr/local/lib -L$PREFIX/lib -Wl,-rpath,/usr/local/lib -Wl,-rpath,$PREFIX/lib"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

arch -x86_64 ./configure \
  --prefix="$PREFIX" \
  --enable-win64 \
  --with-opengl \
  --without-vulkan \
  --without-x

arch -x86_64 make -j"$(sysctl -n hw.ncpu)"
arch -x86_64 make install

find $PREFIX/lib/wine/x86_64-windows \( -name '*.dll' -o -name '*.exe' \) | \
  xargs -P"$(sysctl -n hw.ncpu)" x86_64-w64-mingw32-strip --strip-debug

strip -x $PREFIX/lib/*.dylib
strip -x $PREFIX/lib/wine/x86_64-unix/*.so

rm -rf $PREFIX/include $PREFIX/lib/pkgconfig
