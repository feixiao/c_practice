#include <thread>
#include <iostream>

#include "./thread_safe_queue.h"


// g++ -Wall -std=c++11 6_3.cpp -o 6_3 -lpthread

volatile bool done;

template<typename T>
void Pop(thread_safe_queue<T>& queue) {
    while(!done) {
        T value;
        queue.wait_and_pop(value);
        std::cout << "Pop : " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
    }
}

template<typename T>
void Push(thread_safe_queue<T>& queue) {
     while(!done) {
         T value;
         queue.push(value);
         std::cout << "Push : " << value<< std::endl;
         std::this_thread::sleep_for(std::chrono::milliseconds(5)); 
     }
}

int main() {
    thread_safe_queue<int> queue;

    done = false;

    std::thread t1(Push<int>,std::ref(queue));

    std::thread t2(Pop<int>,std::ref(queue));

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 

    done = true;
    t1.join();
    t2.join();
}