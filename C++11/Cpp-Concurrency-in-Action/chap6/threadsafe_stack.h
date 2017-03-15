#ifndef _THREAD_SAFE_STACK_H_
#define _THREAD_SAFE_STACK_H_

#include <exception>
#include <stack>
#include <mutex>
#include <memory>

/*
 *  线程安全的栈
 */
 
struct empty_stack: std::exception
{
    const char* what() const throw()
    {
        return "empty stack";
    }
};

template<typename T>
class threadsafe_stack
{
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        m_data = other.m_data;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.push(std::move(new_value));
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_data.empty()) throw empty_stack();

        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(m_data.top())));

        m_data.pop();
        return res;
    }

    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_data.empty()) throw empty_stack();
        value=std::move(m_data.top());
        m_data.pop();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data.empty();
    }

private:
    std::stack<T> m_data;
    mutable std::mutex m_mutex;
};



#endif 