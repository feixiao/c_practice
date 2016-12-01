#include <uuid/uuid.h>
#include <string>
#include <iostream>

// g++ -Wall uuid.cpp -o uuid -luuid

using std::string;
using std::cout;
using std::endl; 

string gene_uuid_str()
{
      uuid_t uu;
      uuid_generate(uu); //extern void uuid_generate(uuid_t out);
      char buf[37];
      uuid_unparse(uu, buf);
      return string(buf);
}

int main() {
    cout << gene_uuid_str() << endl;
}

