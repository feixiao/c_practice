## Sanitizer

### 一：简介

[google/**sanitizers** ](https://github.com/google/**sanitizers**  )包括四个部分：AddressSanitizer, MemorySanitizer, ThreadSanitizer, LeakSanitizer。

+ [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer)

  检测常见的内存使用问题，包括：

  + [Use after free](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleUseAfterFree) 
  + [Heap buffer overflow](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleHeapOutOfBounds)
  + [Stack buffer overflow](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleStackOutOfBounds)
  + [Global buffer overflow](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleGlobalOutOfBounds)
  + [Use after return](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleUseAfterReturn)
  + [Use after scope](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleUseAfterScope)
  + [Initialization order bugs](https://github.com/google/sanitizers/wiki/AddressSanitizerInitializationOrderFiasco)
  + [Memory leaks](https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer)

+ [LeakSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer)

  检查内存泄露问题。

+ [MemorySanitizer](https://github.com/google/sanitizers/wiki/MemorySanitizer)

  检测读取没有初始化内存问题。

+ [ThreadSanitizer](https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual)

  检测数据竞争和死锁问题。

**注:** 对比valgrind的优势是sanitizer基本不拖慢程序(~2x), valgrind在（～10x）左右。



###  二：各个平台支持情况

#### Linux C/C++

+ GCC从[4.8版本](https://gccgnu.org/onlinedocs/gcc-4.8.0/gcc/Debugging-Options.html#Debugging-Options)开始支持Address和Thread Sanitizer，[4.9版本](https://gcc.gnu.org/onlinedocs/gcc-4.9.0/gcc/Debugging-Options.html#Debugging-Options)开始支持Leak Sanitizer和UB Sanitizer。
+ Clang 3.2+

#### OSX/IOS

+ XCode 带编译配置选项所以是完全支持的。

#### Android

+ [在AndroidStudio上使用AddressSanitizer](http://www.imooc.com/article/80815)

   虽然使用的Android Studio但是方法是在cmake中添加配置编译选项(make相同方法)。

#### Windows

+ 不支持，但是可以使用免费的[MTuner](https://github.com/milostosic/MTuner)或者付费的Intel Parallel Studio作为替代品。

### 参考资料

+ [wiki](https://github.com/google/sanitizers/wiki)

