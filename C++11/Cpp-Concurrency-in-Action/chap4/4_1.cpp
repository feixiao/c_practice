#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

// 使用 std::condition_variable 处理数据等待
// g++ -Wall -std=c++11 4_1.cpp -o 4_1 -lpthread

bool more_data_to_prepare() {
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); 
    return true;
}

struct data_chunk {

};

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

std::mutex mut;
std::queue<data_chunk> data_queue;  // 一个用来在两个线程之间传递数据的队列
std::condition_variable data_cond;  

void data_preparation_thread() {
    while(more_data_to_prepare()) {
        data_chunk const data = prepare_data();
        // 使用 std::lock_guard 对队列上锁，将准备好的数据压入队列
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        // 调用 std::condition_variable 的notify_one()成员函数，对等待的线程(如果有等待线程)进行通知
        data_cond.notify_one();
    }
}

void data_processing_thread() {
    while(true) {
        std::unique_lock<std::mutex> lk(mut);
        // 线程之后会调用 std::condition_variable 的成员函数wait()，传递一个锁和一个lambda函数表达式(作为等待的条件)
        data_cond.wait(lk,[]{return !data_queue.empty();});
        data_chunk data=data_queue.front();
        data_queue.pop();
        lk.unlock();    // 解锁
        process(data);  // 处理数据
        if(is_last_chunk(data)) {
            break;
        }
    }
}

int main() {
    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);
    
    t1.join();
    t2.join();
}