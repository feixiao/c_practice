#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "timer.h"

// g++ --std=c++11 -o main main.cpp
int main() {
  Timer timer;
  timer.start(1000, []() { std::cout << "Hello, world!" << std::endl; });
  std::this_thread::sleep_for(std::chrono::seconds(1000));
  return 0;
}