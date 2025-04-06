#!/bin/bash

ARCHES=("x86_64" "i686") # "arm64"

build_windows() {
    local arch=$1

    echo "Building for Windows $arch..."
    mkdir -p "$arch"
    cd "$arch"

    cmake ../../../ \
        -DTARGET_PLATFORM=windows \
        -DCMAKE_TOOLCHAIN_FILE="../../../toolchains/windows-${arch}.cmake" \
        -DCMAKE_BUILD_TYPE=Releasec
    make -j$(nproc)

    cd ../
    mkdir "../../out/windows/$arch"
    cp "$arch/stb_image_jni.dll" "../../out/windows/$arch/stb_image_jni.dll"
}

set -e

rm -rf out/windows
mkdir -p out
mkdir out/windows/

mkdir -p build
cd build
mkdir -p windows
cd windows/

for arch in "${ARCHES[@]}"; do
    build_windows "$arch"
done