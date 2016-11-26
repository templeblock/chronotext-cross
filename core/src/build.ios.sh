#!/bin/sh

PLATFORM="ios"

BUILD_DIR="build/$PLATFORM"
INSTALL_DIR="tmp/$PLATFORM/lib/Release"
INSTALL_PATH="$(pwd)/$INSTALL_DIR"

# ---

TOOLCHAIN_FILE="$CROSS_PATH/core/cmake/toolchains/ios.cmake"

cmake . -B"$BUILD_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBRARY_OUTPUT_PATH="$INSTALL_PATH" \
  -DPLATFORM="$PLATFORM"

if [ $? != 0 ]; then
  echo "CONFIGURATION FAILED!"
  exit -1
fi

# ---

rm -rf "$INSTALL_PATH"
cmake --build "$BUILD_DIR"

if [ $? != 0 ]; then
  echo "BUILD FAILED!"
  exit -1
fi

mkdir -p "../../tree/chr/$PLATFORM/lib"
mv "tmp/$PLATFORM/lib/Release" "../../tree/chr/$PLATFORM/lib"
