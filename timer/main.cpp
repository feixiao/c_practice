#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "timer.h"

class Timer {
 public:
  Timer() : expired_(true), is_recurring_(false) {}

  // 设置定时器，单位为毫秒
  void setTimer(int interval, std::function<void()> callback,
                bool is_recurring = false) {
    expired_ = false;
    is_recurring_ = is_recurring;
    interval_ = std::chrono::milliseconds(interval);
    callback_ = std::move(callback);
    timer_thread_ = std::thread([this]() { this->threadFunc(); });
  }

  // 停止定时器
  void stop() {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      expired_ = true;
    }
    cv_.notify_one();
    timer_thread_.join();
  }

  // 定时器线程函数
  void threadFunc() {
    while (true) {
      std::unique_lock<std::mutex> lock(mutex_);
      if (cv_.wait_for(lock, interval_, [this] { return expired_; })) {
        break;
      }

      callback_();

      if (!is_recurring_) break;
    }
  }

 private:
  bool expired_;
  bool is_recurring_;
  std::chrono::milliseconds interval_;
  std::function<void()> callback_;
  std::thread timer_thread_;
  std::mutex mutex_;
  std::condition_variable cv_;
};

int main() {
  Timer timer;

  // 设置定时器，每隔1000ms执行一次回调函数
  timer.setTimer(
      1000, []() { std::cout << "Timer callback called\n"; }, true);

  // 等待10秒后停止定时器
  std::this_thread::sleep_for(std::chrono::seconds(10));
  timer.stop();

  return 0;
}

// g++ --std=c++11 -o main main.cpp
int main() {
  Timer timer;
  timer.start(200, []() {
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 将时间点转换为时间戳（以毫秒为单位）
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                         now.time_since_epoch())
                         .count();

    std::cout << timestamp << " Hello, world!" << std::endl;
  });
  std::this_thread::sleep_for(std::chrono::seconds(5000));
  timer.stop();
  return 0;
}