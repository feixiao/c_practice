### jpeg概述

#### OSX
```shell
brew install libjpeg-turbo

#If you need to have jpeg-turbo first in your PATH, run:
#  echo 'export PATH="/usr/local/opt/jpeg-turbo/bin:$PATH"' >> ~/.zshrc
#
#For compilers to find jpeg-turbo you may need to set:
#  export LDFLAGS="-L/usr/local/opt/jpeg-turbo/lib"
#  export CPPFLAGS="-I/usr/local/opt/jpeg-turbo/include"
#
#For pkg-config to find jpeg-turbo you may need to set:
#  export PKG_CONFIG_PATH="/usr/local/opt/jpeg-turbo/lib/pkgconfig"
```


#### 参考资料
+ [《编译libjpeg-turbo和使用教程》](https://aijishu.com/a/1060000000078362)