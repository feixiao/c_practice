#include "gmock/gmock.h"
#include "foo.h"

class MockFoo : public FooInterface
{
public:
  MOCK_METHOD0(getArbitraryString, std::string());
  MOCK_METHOD0(getPosition, int());
};