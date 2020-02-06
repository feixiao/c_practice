#include <ctime>
#include <iostream>


// g++ -std=c++11 -O3 -Wall -g -o core-dump-demo main.cpp

int main(int argc, char *argv[]) {
 time_t begin_time = time(0);
 time_t cur_time = time(0);
 std::cout << "begin time = " << ctime(&begin_time) << std::endl;
 while ((cur_time - begin_time) < 120) {
        cur_time = time(0);
 }
 std::cout << "end time = " << ctime(&cur_time) << std::endl;
 char *p = nullptr;
 // crash here
 *p = 'x';
 return 0;
}