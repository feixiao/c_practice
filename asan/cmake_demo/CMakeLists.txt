# cmake_minimum_required(VERSION <specify CMake version here>)
project(cmake_demo)

set(CMAKE_CXX_STANDARD 11)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")


#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")


# https://stackoverflow.com/questions/10165335/can-cmake-specify-the-base-sdk-on-mac-os-x
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    #    MACOSX_DEPLOYMENT_TARGET=10.7
    #
    #cmake -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=/Developer/SDKs/MacOSX10.7.sdk/ -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7 ../..
    # set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk")
    # set(CMAKE_CXX_FLAGS "--sysroot ${CMAKE_OSX_SYSROOT} ${CMAKE_CXX_FLAGS}")

    # https://stackoverflow.com/questions/69803659/what-is-the-proper-way-to-build-for-macos-x86-64-using-cmake-on-apple-m1-arm
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
endif ()

add_executable(cmake_demo main.cpp)