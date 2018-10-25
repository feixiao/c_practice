#include <iostream>
#include <map>
#include <string>
#include <memory>

// g++ -Wall main.cpp  -o main  -std=c++11

using namespace std;

class Person {
public:
    Person() = delete;
    Person(std::string name) : _name(name) {
        std::cout<<__FUNCTION__ << std::endl;
    }

    ~Person() {
         std::cout<<__FUNCTION__ << std::endl;
    }

    std::string Name() {
        return _name;
    }
private:
    std::string _name;

};

int main() {
    std::cout<<__FUNCTION__ << std::endl;
    std::map<std::string, std::shared_ptr<Person> > persons;
    auto p1 = make_shared<Person>("frank");   
    std::cout<< "0 p1.use_count : " << p1.use_count() << std::endl;
    persons[p1->Name()] = p1;   // 引用计数 +1 
    std::cout<< "1 p1.use_count : " << p1.use_count() << std::endl;
    auto it = persons.find(p1->Name());
    if( it != persons.end()) {
        std::cout<< "2 p1.use_count : " << p1.use_count() << std::endl;
        persons.erase(it); // 引用计数 -1 
        std::cout<< "3 p1.use_count : " << p1.use_count() << std::endl;
    }

    std::cout<< "main quit" << std::endl;
    return 0;
}