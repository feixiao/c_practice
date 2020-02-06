## Docker环境下面生成coredump文件

### 快速使用
#### 编译程序
```
g++ -std=c++11 -O3 -Wall -g -o core-dump-demo main.cpp
```

#### 构建镜像
```
docker build --rm -f "Dockerfile" -t core-dump-demo:0.0.1 .
```

### 参考资料
+ [《如何在docker容器中生成core dump》](https://zhuanlan.zhihu.com/p/87333063)