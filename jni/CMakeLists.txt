cmake_minimum_required(VERSION 3.10)
project(stb_image_jni)

# platform variable
set(TARGET_OS "linux")

# jdk headers
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(JAVA_HOME "/usr/lib/jvm/java-17-openjdk")
    include_directories("${JAVA_HOME}/include/linux" "${JAVA_HOME}/include")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(JAVA_HOME "C:/Program Files/Java/jdk-17")
    include_directories("${JAVA_HOME}/include/win32" "${JAVA_HOME}/include")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{ANDROID_NDK}/build/cmake/android.toolchain.cmake")
    set(ANDROID_ABI "arm64-v8a")
    set(ANDROID_NATIVE_API_LEVEL 34)
endif()

# add sources
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
include_directories("include" "lib")
add_library(stb_image_jni SHARED ${SOURCE_FILES})

# platform-specific options
if(TARGET_OS STREQUAL "linux")
    message(STATUS "Compiling for Linux")

elseif(TARGET_OS STREQUAL "windows")
    message(STATUS "Compiling for Windows")
    # ensure cmake generates a dll
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

elseif(TARGET_OS STREQUAL "android")
    message(STATUS "Compiling for Android")

    target_link_libraries(stb_image_jni PRIVATE android log)
endif()