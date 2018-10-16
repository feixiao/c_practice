

#### 动态链接库

1：加载的函数需要导出为C函数

```c++
#if defined(__cplusplus) || defined(c_plusplus)
extern "C"{
#endif

void func();


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
```

2:  dlopen等函数需要引入 -ldl 

3：可能动态库会使用可执行程序的符号表

```makefile
$(TARGET):$(EXE_OBJS) 
	$(CPP) -o $@ $^  $(EXTLIBS) $(LDFLAGS) 
# 当动态库需要使用可执行程序的变量或者函数时，我们需要将可执行程序的符号表导出为全局符号表
# $(CPP) -Wl,--export-dynamic -o $@ $^  $(EXTLIBS) $(LDFLAGS)  
```



#### 参考资料

+  [《静态链接库、动态链接库和动态加载库》](https://www.cnblogs.com/nufangrensheng/p/3578784.html) 		
