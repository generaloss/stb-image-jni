set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER /opt/llvm-mingw-arm64/bin/aarch64-w64-mingw32-clang)
set(CMAKE_CXX_COMPILER /opt/llvm-mingw-arm64/bin/aarch64-w64-mingw32-clang++)

set(CMAKE_FIND_ROOT_PATH /opt/llvm-mingw-arm64/aarch64-w64-mingw32)

set(CMAKE_C_FLAGS "-fuse-ld=lld")
set(CMAKE_CXX_FLAGS "-fuse-ld=lld")