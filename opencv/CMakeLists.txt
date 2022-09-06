cmake_minimum_required(VERSION 3.22)
project(opencv)

set(CMAKE_CXX_STANDARD 11)


set(opencv_libs     #项目用到的库
    opencv_core
    opencv_dnn
    opencv_imgproc
    opencv_imgcodecs
    opencv_ml
    opencv_calib3d
    opencv_highgui)


include_directories(
    /usr/local/include/opencv4
)

link_directories(
    /usr/local/lib
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


add_executable(opencv main.cpp)

target_link_libraries( opencv
        ${opencv_libs})