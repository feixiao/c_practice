#include <thread>
#include <iostream>

// g++ -Wall -std=c++11 2_6.cpp -o 2_6 -lpthread

// 获取线程的所有权，确保线程的退出
class scoped_thread {
    std::thread t;
public:
    explicit scoped_thread(std::thread t_):t(std::move(t_)){
        if(!t.joinable()){
            throw std::logic_error("No thread");
        }
    }
    ~scoped_thread() {
        t.join();
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
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
    scoped_thread t(std::thread(func(some_local_state))); 
    // warning: unused variable ‘some_local_state’ [-Wunused-variable]

}

int main(){
    f();
}