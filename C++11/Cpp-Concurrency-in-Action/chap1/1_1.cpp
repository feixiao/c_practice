#include <iostream>
#include <thread>  // 第一个区别是增加了 #include <thread>,标准C++库中对多线程支持的声明在新的头文件中


// g++ -Wall -std=c++11 1_1.cpp -o 1_1 -lpthread

void hello(){
    std::cout << "Hello Concurrent World\n";
}

int main(){
    std::thread t(hello); // 因为每个线程都必须具有一个初始函数(initial function)，新线程的执行在这里开始。
    t.join(); // 等待线程退出
}