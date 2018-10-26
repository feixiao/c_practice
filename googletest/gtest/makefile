# 编译可执行程序

# 工具链
CPP = g++
RM = rm -f
STRIP = strip

# 头文件路径
INC +=  -I./
# 第三方库
EXTLIBS = -lgtest  -lgtest_main

# 编译选项
CFLAGS += -Wall -O2 -Os -g $(INC)

# 系统库
LDFLAGS +=  -lpthread


# 需要编译的文件 
SRCPATH = .
SOURCE += $(SRCPATH)/sample1.cc
SOURCE += $(SRCPATH)/sample1_unittest.cc

EXE_OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

TARGET = ./sample1

all:$(TARGET)

$(TARGET):$(EXE_OBJS) $(EXTLIBS)
	$(CPP) -o $@ $^  $(LDFLAGS)

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(TARGET) $(EXE_OBJS)
