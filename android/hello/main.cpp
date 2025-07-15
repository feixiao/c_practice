#include <chrono>
#include <iostream>
#include <thread>

// 使用android logging library
#include <android/log.h>
// 定义日志标签
#define LOG_TAG "HelloWorld"
// 定义日志级别
#define LOG_LEVEL ANDROID_LOG_INFO
int main() {
  // 输出30次，每隔1s输出一次
  for (int i = 0; i < 30; ++i) {
    // 使用android logging library输出日志
    __android_log_print(LOG_LEVEL, LOG_TAG, "Hello, World!");
    // 等待1秒
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}