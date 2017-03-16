#ifndef _THREADSAFE_QUEUE_H_
#define _THREADSAFE_QUEUE_H_

#include <mutex>
#include <condition_variable>
#include <queue>


// 使用条件变量实现的线程安全队列

template<typename T>
class threadsafe_queue 
{
public:
    threadsafe_queue(){}

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_queue.push(std::move(new_value));
        m_cond.notify_one();  // 1  当不止一个线程等待对队列进行推送操作，只会有一个线程，因得到data_cond.notify_one()，而继续工作着。如果这个工作线程（接收到条件的线程）在wait_and_pop()中抛出一个异常，那么其他线程则会永世长眠。
    }

    void wait_and_pop(T& value) //2
    {
        std::unique_lock<std::mutex> lk(m_mutex); // ？？unique_lock
        m_cond.wait(lk,[this]{return !m_queue.empty();});
        value = std::move(m_queue.front());
        m_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() // 3
    {
        std::unique_lock<std::mutex> lk(m_mutex); // ？？unique_lock
        m_cond.wait(lk,[this]{return !m_queue.empty();}); // 4
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(m_queue.front()))
        );
        m_queue.pop();
        return res;
     }

     std::shared_ptr<T> try_pop() 
     {
        std::lock_guard<std::mutex> lk(m_mutex);
        if(m_queue.empty())
        {
            return std::shared_ptr<T>(); // 5
        }
        else
        {        
            std::shared_ptr<T> res(
                std::make_shared<T>(std::move(m_queue.front()))
            );
            m_queue.pop();
            return res;
        }
     }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        return m_queue.empty();
    }
private:
    mutable std::mutex m_mutex;   
    std::queue<T> m_queue;
    std::condition_variable m_cond;
};

#endif 