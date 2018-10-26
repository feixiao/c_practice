 #ifndef _FOO_H
 #define _FOO_H
 
#include <iostream>

using namespace std;
class FooInterface {
public:
  virtual ~FooInterface() {}
  virtual std::string getArbitraryString() = 0;
  virtual int getPosition() = 0;
};

#endif