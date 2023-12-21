## Android 编译

### 编译

```shell

./ndk_build.sh
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
