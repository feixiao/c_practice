#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

// g++ -Wall -std=c++11 2_8.cpp -o 2_8 -lpthread

template<typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator first,Iterator last,T& result){
        result = std::accumulate(first,last,result);
    }
};


template<typename Iterator,typename T>
T parallel_accumlate(Iterator first,Iterator last,T init) {
    unsigned long const length = std::distance(first,last);
    if( 0 == length) {
        return init;  // 如果输入的范围为空，就会得到init的值。
    }

    unsigned long const min_per_thread = 25; // 每个线程处理25个任务
    // 用范围内元素的总数量除以线程(块)中最小任务数，从而确定启动线程的最大数量，这样能避免无谓的计算资源的浪费
    unsigned long const max_threads = 
            (length + min_per_thread - 1) / min_per_thread; 
    unsigned long const hardware_threads = 
            std::thread::hardware_concurrency();

    // 计算量的最大值和硬件支持线程数中，较小的值为启动线程的数量
    unsigned long const num_threads = 
            std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    // 每个线程中处理的元素数量,是范围中元素的总量除以线程的个数得出的
    unsigned long const block_size = length / num_threads;  

    std::vector<T> results(num_threads);
    // 启动的线程数必须比num_threads少1个，因为在启动之前已经有了一个线程(主线程)
    std::vector<std::thread> threads(num_threads - 1);

    Iterator block_start = first;
    for(unsigned long i=0;i<(num_threads -1);i++){
        Iterator block_end = block_start;
        // block_end迭代器指向当前块的末尾（每个块处理block_size个，所以这个线程的处理范围是block_start,block_start+block_size）
        std::advance(block_end,block_size);
        // 并启动一个新线程为当前块累加结果,保存在results[i]中
        threads[i] = std::thread(
            accumulate_block<Iterator,T>(),
            block_start,block_end,
            std::ref(results[i]));
        // 当迭代器指向当前块的末尾时，启动下一个块
        block_start=block_end;
    }
    // 启动所有线程后，会处理最终块的结果（不放再for循环是因为最后一个块，可能不到block_size）
    accumulate_block<Iterator,T>()(
            block_start,last,results[num_threads-1]);

    //  创建线程的完成之后，等待线程处理完毕
    std::for_each(threads.begin(),threads.end(),
            std::mem_fn(&std::thread::join)); 
            
    // 使用 std::accumulate 将所有结果进行累加
    return std::accumulate(results.begin(),results.end(),init); 
}


int main(){
    std::vector<int> vi;
    for(int i=0;i<10;++i)
    {
        vi.push_back(10);
    }
    int sum=parallel_accumlate(vi.begin(),vi.end(),5);
    std::cout<<"sum="<<sum<<std::endl;
}