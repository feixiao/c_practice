#include <iostream>
#include <string>
#include <mutex>

// 无意中传递了保护数据的引用
// g++ -Wall -std=c++11 3_2.cpp -o 3_2 -lpthread

class some_data {
public:
    void do_something(){};
private:
    int a;
    std::string b;
};

class data_wrapper {
private:
    some_data data;  // 受保护的数据
    std::mutex m;
public:
    template<typename Function>
    void process_data(Function func) {
        std::lock_guard<std::mutex> l(m);
        func(data); // 传递保护数据给用户函数
    }
};

some_data* unprotected;

// 恶意函数
void malicious_function(some_data& protected_data) {
    unprotected = &protected_data;
}

data_wrapper x;
void foo() {
    x.process_data(malicious_function); // 传递恶意函数
    unprotected->do_something(); // 在无保护的情况下访问保护数据
}

int main() {
    foo();
}