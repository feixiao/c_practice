cmake_minimum_required(VERSION 3.22)
project(jpeg2)

set(CMAKE_CXX_STANDARD 11)


include_directories(
        /usr/local/opt/jpeg-turbo/include
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    add_definitions("-Wno-invalid-source-encoding")
    add_definitions("-Os")
    set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk")
    set(CMAKE_CXX_FLAGS "--sysroot ${CMAKE_OSX_SYSROOT} ${CMAKE_CXX_FLAGS}")

    # https://stackoverflow.com/questions/69803659/what-is-the-proper-way-to-build-for-macos-x86-64-using-cmake-on-apple-m1-arm
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set(UNIX_OS true)
    SET(TARGET_OS_NAME "osx")
else ()
    message(FATAL_ERROR "unknown CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
endif ()


add_executable(jpeg2 main.cpp)

target_link_libraries( jpeg2
        /usr/local/opt/jpeg-turbo/lib/libjpeg.a)