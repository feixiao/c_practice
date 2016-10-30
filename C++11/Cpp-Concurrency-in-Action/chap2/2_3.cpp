#include <iostream>
#include <thread>


// g++ -Wall -std=c++11 2_3.cpp -o 2_3 -lpthread

// 使用RAII等待线程完成


class thread_guard {
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_):t(t_) {}
    ~thread_guard() {
        if(t.joinable()) {
            t.join();
        }
    }
    thread_guard(thread_guard const &) = delete; // 编译将不会生成拷贝构造函数
    thread_guard& operator=(thread_guard const &) = delete; // 编译将不会生成赋值函数
};

void do_something(int& i){
    ++i;
}

struct func {
    int& i;

    func(int& i_):i(i_){}
    // 将被线程调用的函数
    void operator()() {
        for(unsigned j=0;j<1000000;++j) {
            std::cout<< "void operator()() : " << i << std::endl;
            do_something(i);
        }
    }
};

void f(){
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
}

int main(){
    f();
}