## Android 编译

### 编译

```shell
export ANDROID_NDK=/mnt/f/wsl/android/android-ndk-r23-linux
export PATH=${PATH}:${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin
rm -r build
mkdir build && cd build

cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
	-DANDROID_ABI="arm64-v8a" \
	-DANDROID_NDK=$ANDROID_NDK \
    -DCMAKE_SYSTEM_NAME=Android \
	-DANDROID_PLATFORM=android-29 \
	..

make && make install

cd ..

# 查看库版本
# arm-linux-androideabi-readelf -a ./libs/android/arm64-v8a/libadd.a
# aarch64-linux-android-readelf -a ./libs/android/arm64-v8a/libadd.a 

```

### 运行
```shell
su
chmod 777 /data/local 
# 然后推送到/data/local运行即可
```
