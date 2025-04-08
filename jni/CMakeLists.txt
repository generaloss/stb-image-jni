cmake_minimum_required(VERSION 3.10)
project(stb_image_jni)

# jdk headers
set(JAVA_HOME "/usr/lib/jvm/java-8-openjdk") # for windows: set(JAVA_HOME "C:/Program Files/Java/jdk-17")
include_directories("${JAVA_HOME}/include/linux" "${JAVA_HOME}/include") # for windows: include_directories("${JAVA_HOME}/include/win32" "${JAVA_HOME}/include")

# sources
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# library
add_library(stb_image_jni SHARED ${SOURCE_FILES})

if(TARGET_PLATFORM STREQUAL "android")
    message(STATUS "Configuring for Android")
    target_link_libraries(stb_image_jni PRIVATE android log)
    set_target_properties(stb_image_jni PROPERTIES SUFFIX ".so")

elseif(TARGET_PLATFORM STREQUAL "windows")
    message(STATUS "Configuring for Windows")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set_target_properties(stb_image_jni PROPERTIES
        SUFFIX ".dll"
        PREFIX ""
        IMPORT_PREFIX ""
    )

    # added for static linking
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    target_link_options(stb_image_jni PRIVATE -static -static-libgcc -static-libstdc++)

else()
    message(STATUS "Configuring for Linux")
    target_compile_options(stb_image_jni PRIVATE -fPIC)
    set_target_properties(stb_image_jni PROPERTIES SUFFIX ".so")
endif()