## GoogleTest快速入门

#### 介绍

+ **Google Test**

  [《Getting started》](https://github.com/google/googletest/blob/master/googletest/docs/primer.md) 中文翻译《[GTest框架简介（译文）](https://www.cnblogs.com/jycboy/p/6057677.html)》

+ **Google Mock**

   [《Google Mock documentation》](https://github.com/google/googletest/blob/master/googlemock/README.md)

   [Google Mock简介--概念及基础语法](https://www.cnblogs.com/jycboy/p/gmock_summary.html)

#### 安装
##### Linux
```c
cmake ./
make -j 4
sudo make install
```
##### Windows
+ [《VS2015搭建GoogleTest框架--配置第一个项目》](http://www.cnblogs.com/jycboy/p/6001153.html)

#### gtest sample

运行程序：

```shell
cd ./gtest 
make -j4 
./test 运行单元测试
```

##### sample1：介绍基本断言的使用
+ [《sample1注解》](https://www.cnblogs.com/jycboy/p/6057849.html)

+ 结构介绍

  ```
  sample1.h   函数声明
  sample1.cc  函数定义
  sample1_unittest.cc  针对sample1.cc中函数的定义设计的单元测试
  ```

##### sample2：

EXPECT_STREQ()用于c_string(以’\0’结尾的c风格字符串)的检查。 如果是string对象,判等用的还是EXPECT_EQ()。

##### sample3：

介绍了test fixture,通过继承 testing::Test来准备和消耗测试的上下文，测试。

[《TestFixture使用》](https://www.cnblogs.com/jycboy/p/gtest_testfixture.html)

##### sample4：

介绍基本断言的使用，通过测试Counter类。

##### sample5：

通过继承 testing::Test来计算测试时间，检查测试是否太慢。

##### sample6：

介绍如何测试不同子类(OnTheFlyPrimeTable和PreCalculatedPrimeTable)对基类的接口的实现。

##### sample7:
介绍了利用参数化方法进行接口测试的方法。允许您使用不同的参数测试代码，而无需编写同一测试的多个副本。主要思路是在fixture中可以通过继承自public TestWithParam来进行参数化测试,参数可以是函数接口或者基类指针等,在fixture中可以通过GetParam得到参数并进行具体的调用测试等. 

[《值参数化测试》](https://www.cnblogs.com/jycboy/p/6118073.html)

##### sample8：

介绍了提供参数的不同组合的方式。sample7为单个参数，这边为多参数。

##### sample9：

介绍了怎样利用其提供的listener自己定制输出内容而不是默认输出。

##### sample10：

Google测试提供了一个事件侦听器API，让您接收有关测试程序进度和测试失败的通知。 

[Extending Google Test by Handling Test Events](https://www.cnblogs.com/jycboy/p/gtest_handlingEvent.html)

#### 参考资料

+ [《GoogleTest单元测试》](https://www.cnblogs.com/jycboy/category/900460.html)
+ [《googletest中samples注解》](https://blog.csdn.net/aganlengzi/article/details/64921496?locationNum=6&fps=1)