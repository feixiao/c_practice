#include <list>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <thread>

// 使用互斥量保护列表

// g++ -Wall -std=c++11 3_1.cpp -o 3_1 -lpthread

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value) {
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find) {
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(),some_list.end(),
                value_to_find) != some_list.end();
}

int main(){
    //add_to_list(42);

    std::vector<std::thread> threads;
    for(int i = 0; i<5; i++) {
        threads.push_back(std::thread(add_to_list,i));
    }

    std::for_each(threads.begin(),threads.end(),
                    std::mem_fn(&std::thread::join)); // 对每个线程调用join()

    std::cout<<"contains(1)="<<list_contains(1)<<", contains(42)="<<list_contains(42)<<std::endl;
}