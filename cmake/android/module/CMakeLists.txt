cmake_minimum_required(VERSION 3.16.0)

project(add)
set(CMAKE_CXX_STANDARD 11)

add_definitions(
    "-W"
    "-fPIC"
    "-DPIC"
    "-Wall"
    "-fpermissive"
    # "-Werror"
    # "-Wshadow"
    "-Wpointer-arith"
    "-Waggregate-return"
    "-Wno-long-long"
    "-Wuninitialized"
    "-D_POSIX_PTHREAD_SEMANTICS"
    "-DFLOATING_POINT"
    "-D__SSE2__"
    "-Wno-narrowing"
    "-DOUTSIDE_SPEEX"
    "-std=c++11"
)

set(lib_output_path ${CMAKE_CURRENT_SOURCE_DIR}/libs/android/${ANDROID_ABI})
set(LIBRARY_OUTPUT_PATH ${lib_output_path})



set(src ${CMAKE_CURRENT_SOURCE_DIR})


set(sources ${src})

foreach(iter ${sources})
    aux_source_directory(${iter} lib_src)
endforeach()


add_library(add STATIC ${lib_src})