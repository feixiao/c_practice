# ACL快速入门

### Linux编译
+ 使用make
  ```
  # 分别进入lib_acl,lib_acl_protocol和lib_acl_cpp 执行make

  #  生成一个库
  cd acl-3.5.0 
  make all
  ```

### Andriod编译
  ```
  cd build
  cmake -DCMAKE_TOOLCHAIN_FILE=/home/frank/android/ndk/android-ndk-r20/build/cmake/android.toolchain.cmake \
	  -DANDROID_ABI="armeabi-v7a" \
	  -DANDROID_ARM_NEON=ON \
	  -DANDROID_PLATFORM=android-16 \
	  -DCMAKE_BUILD_TYPE=RELEASE \
	  -DCMAKE_SYSTEM_NAME=Android \
	  -DACL_CLIENT_ONLY=YES \
	  -DDISABLE_DB=YES \
	  -DHAS_POLARSSL_DLL=YES \
	  ..
```

### 参考资料
+ [acl](https://github.com/acl-dev/acl)
+ [sample](https://github.com/acl-dev/acl/blob/master/SAMPLES.md)