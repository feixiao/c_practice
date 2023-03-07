cmake_minimum_required(VERSION 3.0.0)
project(aes VERSION 0.1.0)

# include(CTest)
# enable_testing()

set(CMAKE_CXX_STANDARD 11)

include_directories(
    /usr/local/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk")
set(CMAKE_CXX_FLAGS "--sysroot ${CMAKE_OSX_SYSROOT} ${CMAKE_CXX_FLAGS}")

# https://stackoverflow.com/questions/69803659/what-is-the-proper-way-to-build-for-macos-x86-64-using-cmake-on-apple-m1-arm
set(CMAKE_OSX_ARCHITECTURES "x86_64")


add_executable(${PROJECT_NAME} main.cpp encryptor.cpp)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)



target_link_libraries(${PROJECT_NAME} PRIVATE  z /usr/local/lib/libmbedcrypto.a)
