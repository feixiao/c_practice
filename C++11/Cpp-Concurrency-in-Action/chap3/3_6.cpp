#include <iostream>
#include <mutex>
#include <string>
#include <thread>

// 交换操作中使用 std::lock() 和 std::lock_guard

// g++ -Wall  -g -std=c++11 3_6.cpp -o 3_6 -lpthread
// -g 方便gdb调试，我们通过gdb的info thread 可以看到线程在相互等锁出现死锁的情况

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
    friend void swap(X& lhs,X& rhs,bool use_stdlock) {
        if(&lhs == &rhs) {
            return;
        }
        if(use_stdlock) {
            // std::lock ——可以一次性锁住多个(两个以上)的互斥量，并且没有副作用(死锁风险),避免多个锁上锁不一致引起的死锁问题
            std::lock(lhs.m,rhs.m);   
        }else {
            lhs.m.lock();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));  // 增加死锁出现的概率
            rhs.m.lock();
        }

        // 提供 std::adopt_lock 参数除了表示 std::lock_guard 对象已经上锁外，还表示现成的锁，而非尝试创建新的锁。
        std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock);
        std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock);
        swap(lhs.some_detail,rhs.some_detail);
    }
private:
    some_big_object some_detail;
    std::mutex m;
    bool   use_stdlock;
};

void mswap(X& lhs,X& rhs,int count,bool use_stdlock) {
    for(int i = 0; i < count; i++) {
        swap(lhs,rhs,use_stdlock);
    }
}

int main() {
    some_big_object o1("o1");
    some_big_object o2("o2");

    X x1(o1);
    X x2(o2);

    // 使用std::lock同时上锁
    std::thread th1(mswap,std::ref(x1),std::ref(x2),10,true);
    std::thread th2(mswap,std::ref(x2),std::ref(x1),10,true);
    th1.join();
    th2.join();

    // 分开上锁，故意交换上锁顺序，会出现死锁
    std::thread th3(mswap,std::ref(x1),std::ref(x2),10,false);
    std::thread th4(mswap,std::ref(x2),std::ref(x1),10,false);
    th3.join();
    th4.join();
}