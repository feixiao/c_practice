# CMake最低版本号要求
cmake_minimum_required(VERSION 3.16.0)

# 项目信息
project (helloworld)

# 设置目标文件的名字
set(TARGET_NAME helloworld)


add_subdirectory(module)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/module)


set()

# 生成可执行文件
add_executable(${TARGET_NAME}
                main.cpp
)


target_link_libraries(${TARGET_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/android/${ANDROID_ABI}/libadd.a
        )