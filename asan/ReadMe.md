# asan快速入门

#### 使用

添加编译选项

```shell
CFLAGS += -fsanitize=address 
```

添加依赖库

```shell
LDFLAGS += -lasan
```

#### 错误

```shell
ASan runtime does not come first in initial library list; you should either link runtime to your application or manually preload it with LD_PRELOAD.
```

##### 解决方案：

```shell
ldd main # 查看链接库位置
libasan.so.2 => /usr/lib/x86_64-linux-gnu/libasan.so.2 (0x00007f651b0a0000)
export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libasan.so.2

./main  # 问题解决
```

[http://blog.sina.com.cn/s/blog_5423c45a0102xmvi.html](http://blog.sina.com.cn/s/blog_5423c45a0102xmvi.html)

#### 参考资料

+ [《Linux下内存检测工具：asan》](https://blog.csdn.net/hanlizhong85/article/details/78076668?locationNum=2&fps=1)

+ [AddressSanitizer](https://en.wikipedia.org/wiki/AddressSanitizer)