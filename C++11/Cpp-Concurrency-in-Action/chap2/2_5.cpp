#include <thread>
#include <iostream>

// g++ -Wall -std=c++11 2_5.cpp -o 2_5 -lpthread

// 返回std::thread 

void some_function() {
    std::cout << "void some_function() called" << std::endl;
}

void some_other_function(int i){
     std::cout << "void some_function() called i=" <<i<<std::endl;
}

std::thread f(){
    return std::thread(some_function);
}
std::thread g() {
    std::thread t(some_other_function,42);
    return t;
}

int main(){
    std::thread t1 = f();
    t1.join();
    std::thread t2 = g();
    t2.join();
}