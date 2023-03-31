cmake_minimum_required(VERSION 3.0.0)
project(test VERSION 0.1.0)



set(CMAKE_CXX_STANDARD 11)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    #	add_definitions("-Wno-invalid-source-encoding")
    add_definitions("-O2")
    set(UNIX_OS true)
    SET(TARGET_OS_NAME "linux")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    add_definitions("-Wno-invalid-source-encoding")
    add_definitions("-Os")
    set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk")
    set(CMAKE_CXX_FLAGS "--sysroot ${CMAKE_OSX_SYSROOT} ${CMAKE_CXX_FLAGS}")

    include_directories(${CMAKE_OSX_SYSROOT}/usr/include)
    # https://stackoverflow.com/questions/69803659/what-is-the-proper-way-to-build-for-macos-x86-64-using-cmake-on-apple-m1-arm
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set(UNIX_OS true)
    SET(TARGET_OS_NAME "osx")
else ()
    message(FATAL_ERROR "unknown CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
endif ()


add_subdirectory(aes)
add_subdirectory(mbedtls/crypt_and_hash)
add_subdirectory(mbedtls/padding)
#add_subdirectory(barrier)
add_subdirectory(barrier_cpp11)