#include <iostream>
#include <vector>
#include <pthread.h>

const int kNumThreads = 4;
const int kNumTasks = 10;

pthread_barrier_t barrier;

void *worker(void *arg)
{
    int id = *(int *)arg;
    for (int i = 0; i < kNumTasks; ++i)
    {
        std::cout << "Thread " << id << " is working on task " << i << std::endl;
        // 模拟计算工作
        for (int j = 0; j < 100000000; ++j)
        {
        }
        // 在Barrier处同步
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

int main()
{
    // 初始化Barrier
    pthread_barrier_init(&barrier, NULL, kNumThreads);

    // 创建线程
    std::vector<pthread_t> threads(kNumThreads);
    std::vector<int> ids(kNumThreads);
    for (int i = 0; i < kNumThreads; ++i)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    // 等待所有线程结束
    for (int i = 0; i < kNumThreads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    // 销毁Barrier
    pthread_barrier_destroy(&barrier);

    return 0;
}
