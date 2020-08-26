#include <iostream>


// cmake -DCMAKE_BUILD_TYPE=Debug ../
// cmake -DCMAKE_BUILD_TYPE=Release ../
int main() {
    auto p = malloc(7);
    p = 0; // The memory is leaked here.

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
