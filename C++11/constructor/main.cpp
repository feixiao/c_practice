#include <iostream>

using  std::ostream;
using  std::cout;
using  std::endl;

// 验证构造函数给成员变量赋值晚于内部赋值

class Person{
public:
    Person(int age) : _age(age) {};
    ~Person() = default;
    int Age() const { return _age;};

    friend  ostream& operator<<(ostream& os,const Person& p) {
        os << "Age : " << p.Age();
        return os;
    }
private:
    int _age = 10;  
};

// g++ -Wall main.cpp -o main

int main() {
    Person p(8);
    cout << p << endl;
}