#!/bin/bash

# set NDK & MINGW
export ANDROID_NDK=/home/user/Dev/sdk/android/ndk/29.0.13113456
export MINGW_TOOLCHAIN=./mingw-toolchain.cmake

# interrupt when errors occurred
set -e

# clean up previous build
rm -rf build-*
mkdir -p build-{linux,windows,android}

# linux
echo "Building for linux..."
mkdir -p build-linux
cd build-linux
cmake .. \
    -DTARGET_PLATFORM=linux \
    -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cd ..

# windows (mingw)
echo "Building for windows..."
if [ -f "$MINGW_TOOLCHAIN" ]; then
    cd build-windows
    cmake .. \
        -DTARGET_PLATFORM=windows \
        -DCMAKE_TOOLCHAIN_FILE=$MINGW_TOOLCHAIN \
        -DCMAKE_BUILD_TYPE=Releasec
    make -j$(nproc)
    cd ..
else
    echo "Skipping windows build: mingw-toolchain.cmake not found"
fi

# android
echo "Building for android arm64-v8a..."
if [ -d "$ANDROID_NDK" ]; then
    cd build-android
    cmake .. \
        -DTARGET_PLATFORM=android \
        -DANDROID_NDK="$ANDROID_NDK" \
        -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI=arm64-v8a \
        -DANDROID_NATIVE_API_LEVEL=24 \
        -DCMAKE_BUILD_TYPE=Release
    make -j$(nproc)
    cd ..
else
    echo "Skipping android build: ANDROID_NDK not set"
fi

echo "Build completed!"