#include <iostream>
#include <thread>
#include <atomic>
#include <algorithm>
#include <exception>
#include <future>
#include "../chap6/thread_safe_queue.h"


// 可等待任务的线程池
// g++ -Wall -std=c++11 9_2.cpp -o 9_2 -lpthread


class function_wrapper
{
public:
    template<typename F>
    function_wrapper(F&& f) : 
        impl(new impl_type<F>(std::move(f))){}
    
    void operator()() 
    {
        impl->call();
    }

    function_wrapper() = default;

    function_wrapper(function_wrapper&& other)
        : impl(std::move(other.impl))
    {

    }
    function_wrapper& operator=(function_wrapper&& other)
    {
        impl = std::move(other.impl);
        return *this;
    }

    function_wrapper(const function_wrapper&)=delete;
    function_wrapper(function_wrapper&)=delete;
    function_wrapper& operator=(const function_wrapper&)=delete;

private:
    struct impl_base 
    {
        virtual void call() = 0;
        virtual ~impl_base(){}
    };

    std::unique_ptr<impl_base> impl;
    template<typename F>
    struct impl_type:impl_base
    {
        F f;
        impl_type(F&& f_):f(std::move(f_)){}
        void call() {f();}
    };
};

class thread_pool
{
public:
        

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) // 1
    {
        typedef typename std::result_of<FunctionType()>::type   result_type;
        std::packaged_task<result_type> task(std::move(f));  // 3
        std::future<result_type> res(task.get_future());    // 4
        work_queue.push(std::move(task));       // 5
        return res;
    } 


private:
    thread_safe_queue<function_wrapper> work_queue;
    std::atomic_bool done;
    void worker_thread() 
    {
        while(!done)
        {
            function_wrapper task;
            if(work_queue.try_pop(task)) 
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
};

int main()
{
    return 0;
}