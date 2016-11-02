#include <thread>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

// g++ -Wall -std=c++11 2_7.cpp -o 2_7 -lpthread

// 量产线程，等待它们结束

void do_work(int i) {
    std::cout << i << std::endl;
}

void f(){
    std::vector<std::thread> threads;
    for(int i = 0; i < 20; i++) {
        threads.push_back(std::thread(do_work,i));
    }

    std::for_each(threads.begin(),threads.end(),
                    std::mem_fn(&std::thread::join)); // 对每个线程调用join()
}

int main(){
    f();
}