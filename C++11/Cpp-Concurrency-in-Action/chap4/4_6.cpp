#include <iostream>
#include <future>
#include <thread>

// 使用 std::future 从异步任务中获取返回值
// g++ -Wall -std=c++11 4_6.cpp -o 4_6 -lpthread

int find_the_answer_to_ltuae(){
     std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
     return 666;
};
void do_other_stuff(){
    std::cout<<"do_other_stuff "<<std::endl;
};
int main() {
    std::future<int> the_answer = std::async(find_the_answer_to_ltuae);
    do_other_stuff();
    std::cout<<"The answer is "<<the_answer.get()<<std::endl;
}