#!/bin/bash

BUILD="build"
DEBUGGER="gdb"
TARGET="main"

if [ "$1" = "debug" ]; then
  ${DEBUGGER} ./build/${TARGET}
else
  cmake --build ${BUILD} -j$(nproc) && ./${BUILD}/${TARGET} > image.ppm && magick image.ppm image.jpg
fi

# vim: ft=sh ts=2 sw=2 et
