#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

// 在该示例中，我们使用std::condition_variable和std::mutex实现了一个Barrier类，并使用wait函数在Barrier处同步线程的执行。
// 在worker函数中，我们使用一个循环来模拟计算工作，并在Barrier处同步线程的执行。
// 在main函数中，我们创建多个线程，并在每个线程中调用worker函数。最后，我们等待所有线程结束并返回。

// 需要注意的是，使用std::condition_variable和std::mutex实现Barrier需要注意正确的线程同步，否则可能会导致死锁或其他并发问题。
// 在实际使用中，建议使用现有的Barrier库，例如Boost.Thread库或C++ 20中引入的std ::barrier。
class Barrier
{
public:
    explicit Barrier(int num_threads) : num_threads_(num_threads), count_(0) {}
    void wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        if (count_ == num_threads_)
        {
            count_ = 0;
            cv_.notify_all();
        }
        else
        {
            // cv_.wait(lock, [this]
            //          { return count_ == 0; });

            bool result = cv_.wait_for(lock, std::chrono::milliseconds(5000),
                                       [this]
                                       { return count_ == 0; });
            if (!result)
            {
                count_ = 0; // 重置计数器
            }
        }
    }

private:
    int num_threads_;
    int count_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

const int kNumThreads = 4;
const int kNumTasks = 2;

Barrier barrier(kNumThreads);

std::mutex gMutex;
// 具体地，每个线程执行kNumTasks个任务，每个任务的编号从0到kNumTasks - 1。当线程完成一个任务后，它会在Barrier处等待其他线程完成同样的任务。
void worker(int id)
{
    for (int i = 0; i < kNumTasks; ++i)
    {
        gMutex.lock();
        std::cout << "Thread " << id << " is working on task " << i << std::endl;
        gMutex.unlock();

        // 模拟计算工作
        for (int j = 0; j < 100000000; ++j)
        {
        }
        // 在Barrier处同步
        barrier.wait();
    }
}

int main()
{
    // 创建线程
    std::vector<std::thread> threads;
    for (int i = 0; i < kNumThreads; ++i)
    {
        threads.emplace_back(worker, i);
    }

    // 等待所有线程结束
    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}
