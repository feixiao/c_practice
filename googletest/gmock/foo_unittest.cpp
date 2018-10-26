#include "mock_foo.h"


using ::testing::Return;

TEST(MockTest, Foo) {
  int n = 100;
  string value = "Hello World!";
  // 创建Mock对象
  MockFoo mockFoo;
  // 启动该对象调用getArbitraryString函数返回value
  EXPECT_CALL(mockFoo, getArbitraryString())
      .Times(1)
      .WillOnce(Return(value));
  // 调用getArbitraryString函数
  string returnValue = mockFoo.getArbitraryString();
  cout << "Returned Value: " << returnValue << endl;
}