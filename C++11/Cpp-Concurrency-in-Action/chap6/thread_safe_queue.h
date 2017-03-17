#ifndef _THREAD_SAFE_QUEUE_H_
#define _THREAD_SAFE_QUEUE_H_

#include <mutex>
#include <condition_variable>
#include <queue>

/*
 * 第三种替代方案就是，将 std::shared_ptr<> 的初始化过程移到push()中，并且存储 std::shared_ptr<> 实例，而非直接使用数据的值。
 * 将 std::shared_ptr<> 拷贝到内部 std::queue<> 中，就不会抛出异常了，这样wait_and_pop()又是安全的了。
 */

template<typename T>
class thread_safe_queue
{
public:
    thread_safe_queue(){}
    void push(T new_value)
    {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
        std::lock_guard<std::mutex> lk(m_mutex);
        m_queue.push(std::move(data));
        m_cond.notify_one();  

    }

    void wait_and_pop(T& value) 
    {
        std::unique_lock<std::mutex> lk(m_mutex); 
        m_cond.wait(lk,[this]{return !m_queue.empty();});
        value = std::move(*m_queue.front());
        m_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() // 3
    {
        std::unique_lock<std::mutex> lk(m_mutex); // ？？unique_lock
        m_cond.wait(lk,[this]{return !m_queue.empty();}); // 4
        std::shared_ptr<T> res = m_queue.front();
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
        std::shared_ptr<T> res = m_queue.front();
        m_queue.pop();
        return res;
     }

     bool try_pop(T& value)
     {
        std::lock_guard<std::mutex> lk(m_mutex);
        if(m_queue.empty())
            return false;
        value=std::move(*m_queue.front());
        m_queue.pop();
        return true;
     }

     bool empty() const 
     {
        std::lock_guard<std::mutex> lk(m_mutex);
        return m_queue.empty();
     }

private:
    mutable std::mutex m_mutex;
    // std::shared_ptr<> 持有数据的好处：
    //      新的实例分配结束时，不会被锁在push()⑤当中(而在清单6.2中，只能在pop()持有锁时完成)。
    //      因为内存分配操作的需要在性能上付出很高的代价(性能较低)，所以使用 std::shared_ptr<> 的方式对队列的性能有很大的提升，其减少了互斥量持有的时间，允许其他线程在分配内存的同时，对队列进行其他的操作。
    std::queue<std::shared_ptr<T> > m_queue;
    std::condition_variable m_cond;
};

#endif 