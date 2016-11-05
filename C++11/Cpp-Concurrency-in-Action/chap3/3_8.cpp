#include <iostream>
#include <mutex>
#include <limits.h>
#include <exception>

// 简单的层级互斥量实现
// g++ -Wall  -g -std=c++11 3_8.cpp -o 3_8 -lpthread
class hierarchical_mutex {
public:
    explicit hierarchical_mutex(unsigned long value):
        hierarchy_value(value),previous_hierarchy_value(0){}
    
    void lock(){
        check_for_hierarchy_violation();
        internal_mutex.lock(); // 在这种检查方式下，lock()代表内部互斥锁已被锁住
        update_hierarchy_value(); // 一旦成功锁住，你可以更新层级值了
    } 
    void unlock(){  
        // 可以调用unlock()对层级值进行保存；否则，就锁不住任何互斥量(第二个互斥量的层级数高于第一个互斥量)，即使线程没有持有任何锁。
        this_thread_hierarchy_value = previous_hierarchy_value; // 6
        internal_mutex.unlock();
    } 
    
    bool try_lock(){
        check_for_hierarchy_violation();
        if(!internal_mutex.try_lock()) 
            return false;
        update_hierarchy_value();
        return true;
    }
private:
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;     // 当前想要上锁的级别
    unsigned long previous_hierarchy_value;  // 上次上锁的级别

    static thread_local unsigned long this_thread_hierarchy_value;  // 使用了thread_local的值来代表当前线程的层级值

    // 检查能否上锁
    void check_for_hierarchy_violation() {
        if(this_thread_hierarchy_value <= hierarchy_value) {
            throw std::logic_error("mutex hierarchy violated");
        }
    }

    // 上锁之后更新当前锁定的级别
    void update_hierarchy_value() {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }
};

thread_local unsigned long
    hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);  // 被初始化为最大值，所以最初所有线程都能被锁住。

int main()
{
    try {
            hierarchical_mutex m1(42);
            hierarchical_mutex m2(2000);
            {
                std::lock_guard<hierarchical_mutex> g1(m1);
                std::lock_guard<hierarchical_mutex> g2(m2);
            }
    }catch(std::exception& ex) {
        std::cout << ex.what() << std::endl;
    } 
}