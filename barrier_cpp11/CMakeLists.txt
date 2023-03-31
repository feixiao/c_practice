cmake_minimum_required(VERSION 3.10)

project(barrier_demo_cpp11)

#include_directories(${CMAKE_OSX_SYSROOT}/usr/include)

# 添加可执行文件
add_executable(barrier_demo_cpp11 main.cpp)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)

# 添加线程库
find_package(Threads REQUIRED)
target_link_libraries(barrier_demo_cpp11 PRIVATE Threads::Threads)
