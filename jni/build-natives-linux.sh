#!/bin/bash

ARCHES=("x86_64" "aarch64" "riscv64") # "arm32" "powerpc64"

build_linux() {
    local arch=$1

    echo "Building for Linux $arch..."
    mkdir -p "$arch"
    cd "$arch"

    cmake ../../../ \
        -DTARGET_PLATFORM=linux \
        -DCMAKE_TOOLCHAIN_FILE="../../../toolchains/linux-${arch}.cmake" \
        -DCMAKE_BUILD_TYPE=Release
    make -j$(nproc)

    cd ../
    mkdir "../../out/linux/$arch"
    cp "$arch/libstb_image_jni.so" "../../out/linux/$arch/libstb_image_jni.so"
}

set -e

rm -rf out/linux
mkdir -p out
mkdir out/linux/

mkdir -p build
cd build
mkdir -p linux
cd linux/

for arch in "${ARCHES[@]}"; do
    build_linux "$arch"
done