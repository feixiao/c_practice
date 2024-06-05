#include <iostream>

// 编译
// cmake -DCMAKE_BUILD_TYPE=Debug ../  -DCMAKE_CXX_COMPILER=clang++
// -DCMAKE_C_COMPILER=clang cmake -DCMAKE_BUILD_TYPE=Release ../

// 运行
// ASAN_OPTIONS=detect_leaks=1 ./cmake_demo
int main() {
  auto p = malloc(7);
  p = 0;  // The memory is leaked here.

  std::cout << "Hello, World!" << std::endl;
  return 0;
}
