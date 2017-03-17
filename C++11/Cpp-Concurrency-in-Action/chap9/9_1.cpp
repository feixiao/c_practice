#include <iostream>
#include <thread>
#include <atomic>
#include <algorithm>
#include <exception>
#include "../chap6/thread_safe_queue.h"

// 简单线程池
// g++ -Wall -std=c++11 9_1.cpp -o 9_1 -lpthread

/*
class join_threads
{
public:
    join_threads(std::vector<std::thread>&){};     
};
*/

class thread_pool 
{
public:
    thread_pool():
        done(false)
    {
        unsigned const thread_count = std::thread::hardware_concurrency();  // 8
        std::cout<< "thread num : " << thread_count << std::endl;
        try
        {   
            for(unsigned i=0;i<thread_count;++i)
            {
                threads.push_back(std::thread(&thread_pool::worker_thread,this)); // 9
            }
        }
        catch(std::exception & ex)
        {
            std::cout<< ex.what() << std::endl;
            done=true;//10
        }
    }
    ~thread_pool()
    {
        done = true;
        std::for_each(threads.begin(),threads.end(),
                    std::mem_fn(&std::thread::join)); 
    }

    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(std::function<void()>(f));
    }
private:
    std::atomic_bool done;
    thread_safe_queue<std::function<void()> > work_queue; //1
    std::vector<std::thread> threads;       //2

    //join_threads joiner;    // 3

    void worker_thread()
    {
        while(!done)    //4
        {
            std::function<void()> task;
            if(work_queue.try_pop(task)) // 5
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

void fa() {
    std::cout<< "fa() enter" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000)); 
    std::cout<< "fa() out" << std::endl;
}

void fb() {
    std::cout<< "fb() enter"  << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000)); 
    std::cout<< "fb() out" << std::endl;
}

int main() {
    thread_pool  pool;
    pool.submit<void()>(fa);
    pool.submit<void()>(fb);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
}   