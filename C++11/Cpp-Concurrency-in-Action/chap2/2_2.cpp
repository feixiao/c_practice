#include <iostream>
#include <thread>

// g++ -Wall -std=c++11 2_2.cpp -o 2_2 -lpthread

// 等待线程退出

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

void do_something_in_current_thread(){

}

void f() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try {
        do_something_in_current_thread();
    }catch(...){
        t.join();
        throw;
    } // 使用了 try/catch 块确保访问本地状态的线程退出后，函数才结束。
    t.join();
}

int main() {
    f();
}