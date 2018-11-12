
#include <iostream>

using namespace std;
 
// http://www.cppblog.com/markqian86/archive/2018/04/17/215594.html
// http://blog.sina.com.cn/s/blog_5423c45a0102xmvi.html


 // g++ -g -O -fsanitize=address -o asan main.cpp
int main(int argc, char **argv) {
  int *array = new int[100];
  delete [] array;
  array[argc]==1;  //can't detected
  cout<<"passed 1st"<<endl;
 
  array[argc] = array[argc];
  cout<< "passed 2nd"<< endl;
 
  array[argc]=100;  // BOOM
  cout<<"passed 3rd"<< endl;
}