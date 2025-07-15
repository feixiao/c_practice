#!/usr/bin/env bash


# For Ubuntu
# for fx compile enviroment
#export ANDROID_NDK=/opt/ndk/android-ndk-r26d
#export PATH=${PATH}:${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin

# For OSX, 下载好的NDK直接拖到Applications目录下
export ANDROID_NDK=/Applications/AndroidNDK12479018.app/Contents/NDK
export PATH=${PATH}:${ANDROID_NDK}/toolchains/llvm/prebuilt/darwin-x86_64/bin

WORKSPACE=$(cd "$dirname "$0")";pwd)
#APP_ABIs="arm64-v8a armeabi-v7a"
APP_ABIs="arm64-v8a"

rm -rf $WORKSPACE/build

for APP_ABI in ${APP_ABIs}
do

cd $WORKSPACE
mkdir android_build_${APP_ABI} && cd android_build_${APP_ABI}

cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
 -DANDROID_ABI=${APP_ABI} \
 -DANDROID_PLATFORM=android-29 \
 -DCMAKE_SYSTEM_NAME=Android \
 -DCMAKE_BUILD_TYPE=RELEASE \
 -DTARGET_OS_NAME=Android \
 ../
make -j4

done

