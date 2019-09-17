#include <memory>
#include <iostream>
 
// https://zh.cppreference.com/w/cpp/memory/enable_shared_from_this
// g++ -std=c++0x -Wall main.cpp -o main
struct Good: std::enable_shared_from_this<Good> // 注意：继承
{
    std::shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};
 
struct Bad
{
    // 错误写法：用不安全的表达式试图获得 this 的 shared_ptr 对象
    std::shared_ptr<Bad> getptr() {
        return std::shared_ptr<Bad>(this);
    }
    ~Bad() { std::cout << "Bad::~Bad() called\n"; }
};
 
int main()
{
    // 正确的示例：两个 shared_ptr 对象将会共享同一对象
    std::shared_ptr<Good> gp1 = std::make_shared<Good>();
    std::shared_ptr<Good> gp2 = gp1->getptr();
    std::cout << "gp1.use_count() = " << gp1.use_count() << '\n';
    std::cout << "gp2.use_count() = " << gp2.use_count() << '\n';
 
    // 错误的使用示例：调用 shared_from_this 但其没有被 std::shared_ptr 占有
    try {
        Good not_so_good;
        std::shared_ptr<Good> gp1 = not_so_good.getptr();
    } catch(std::bad_weak_ptr& e) {
        // C++17 前为未定义行为； C++17 起抛出 std::bad_weak_ptr 异常
        std::cout << e.what() << '\n';    
    }
 
    // 错误的示例，每个 shared_ptr 都认为自己是对象仅有的所有者
    std::shared_ptr<Bad> bp1 = std::make_shared<Bad>();
    std::shared_ptr<Bad> bp2 = bp1->getptr();
    std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';
} // UB ： Bad 对象将会被删除两次