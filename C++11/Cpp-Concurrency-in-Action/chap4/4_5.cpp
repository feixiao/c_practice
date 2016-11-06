#include <memory>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <queue>

// 线程安全队列
// g++ -Wall -std=c++11 4_5.cpp -o 4_5 -lpthread

template<typename T>
class threadsafe_queue {
public:
    threadsafe_queue(){};
    threadsafe_queue(const threadsafe_queue&);
    threadsafe_queue& operator=(const threadsafe_queue&)=delete;

    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();  // 通知等待的线程
    }

    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();}); // 等待通知
        value=data_queue.front();
        data_queue.pop();
    }
    
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value=data_queue.front();
        data_queue.pop();
        return true;
    } 
    
    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>(); // 空就返回空指针
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    } 
    
    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
};

struct data_chunk {};

threadsafe_queue<data_chunk> data_queue; // 1

bool more_data_to_prepare() {
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); 
    return true;
}

data_chunk prepare_data(){
    std::cout << "prepare_data()" << std::endl;
    return data_chunk();
}

void process(data_chunk&){
     std::cout << "process()" << std::endl;
}

bool is_last_chunk(data_chunk&){
    return false;
}

void data_preparation_thread() {
    while(more_data_to_prepare()) {
        data_chunk const data=prepare_data();
        data_queue.push(data);
    }
}

void data_processing_thread() {
    while(true) {
        data_chunk data;
        data_queue.wait_and_pop(data);
        process(data);
        if(is_last_chunk(data))
            break;
    }
}

int main() {
    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);
    
    t1.join();
    t2.join();
}