#include <thread>
#include <iostream>

#include "./threadsafe_stack.h"

using namespace std;

// g++ -Wall -std=c++11 6_1.cpp -o 6_1 -lpthread

volatile bool done;

template<typename T>
void Pop(threadsafe_stack<T>& stack) {
    while(!done) {
        try{
            std::cout << "Pop : " << *stack.pop() << std::endl;
        }catch(empty_stack& ex){
            std::cout<< ex.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
    }
}

template<typename T>
void Push(threadsafe_stack<T>& stack) {
     while(!done) {
         T value;
         stack.push(value);
         std::cout << "Push : " << value<< std::endl;
         std::this_thread::sleep_for(std::chrono::milliseconds(5)); 
     }
}

int main() {
    threadsafe_stack<int> stack;

    done = false;

    std::thread t1(Push<int>,std::ref(stack));

    std::thread t2(Pop<int>,std::ref(stack));

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 

    done = true;
    t1.join();
    t2.join();
}