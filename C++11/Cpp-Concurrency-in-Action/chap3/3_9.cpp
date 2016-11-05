#include <iostream>
#include <mutex>
#include <string>
#include <thread>

// 交换操作中 std::lock() 和 std::unique_lock 的使用
// g++ -Wall  -g -std=c++11 3_9.cpp -o 3_9 -lpthread

class some_big_object{
public:
    some_big_object(std::string name ):name(name) {}
    std::string name;
};

void swap(some_big_object& lhs,some_big_object& rhs){
    std::cout << " swap: " << lhs.name << " " <<  rhs.name << std::endl;
}

class X {
public:
    X(some_big_object const& sd):some_detail(sd){}
    friend void swap(X& lhs,X& rhs) {
        if(&lhs == &rhs) {
            return;
        }
        std::unique_lock<std::mutex> lock_a(lhs.m,std::defer_lock);
        std::unique_lock<std::mutex> lock_b(rhs.m,std::defer_lock);
         // std::lock可以一次性锁住多个(两个以上)的互斥量，并且没有副作用(死锁风险),避免多个锁上锁不一致引起的死锁问题
        std::lock(lock_a,lock_b);
        swap(lhs.some_detail,rhs.some_detail);
    }
private:
    some_big_object some_detail;
    std::mutex m;
};

void mswap(X& lhs,X& rhs,int count) {
    for(int i = 0; i < count; i++) {
        swap(lhs,rhs);
    }
}

int main() {
    some_big_object o1("o1");
    some_big_object o2("o2");

    X x1(o1);
    X x2(o2);

    // 使用std::lock同时上锁
    std::thread th1(mswap,std::ref(x1),std::ref(x2),10);
    std::thread th2(mswap,std::ref(x2),std::ref(x1),10);
    th1.join();
    th2.join();
}