#include <memory>
#include <iostream>
#include <exception>
#include <stack>
#include <mutex>
#include <thread>

// g++ -Wall -std=c++11 3_5.cpp -o 3_5 -lpthread

struct empty_stack: std::exception {
    const char* what() const throw() {
        return "empty stack!";
    }
};

template<typename T> 
class threadsafe_stack {
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack():data(std::stack<T>()){}
    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> l(m);
        data = other.data;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value) {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
        std::cout << " push : " << new_value << std::endl;
    }

    std::shared_ptr<T> pop() {
         std::lock_guard<std::mutex> lock(m);
         if(data.empty()) {
             throw empty_stack();
         }
         std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
         data.pop();
         return res;
    }

    void pop(T& value) {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) {
             throw empty_stack();
         }
         value = data.top();
         data.pop();
         std::cout << " pop : " << value << std::endl;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

template<typename T>
void push_data(threadsafe_stack<T>& stack,int cnt) {
        for(int i = 0; i < cnt; i++) {
            stack.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(20)); 
        }
}

template<typename T>
void pop_data(threadsafe_stack<T>& stack,int cnt) {
        for(int i = 0; i < cnt; i++) {
            try{
                int value = 0;
                stack.pop(value);
                std::this_thread::sleep_for(std::chrono::milliseconds(20)); 
            }catch(std::exception& ex) {
                std::cout << ex.what() << std::endl;
            }
        }
}

int main()
{
    /*threadsafe_stack<int> si;
    si.push(5);
    si.pop();
    if(!si.empty())
    {
        int x;
        si.pop(x);
    }*/

    threadsafe_stack<int> si;
    std::thread thread_push(push_data<int>,std::ref(si),100);
    std::thread thread_pop(pop_data<int>,std::ref(si),100);

    thread_push.join();
    thread_pop.join();
}