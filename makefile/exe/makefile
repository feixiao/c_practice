# 编译可执行程序

# 工具链
CPP = g++
CC = gcc
RM = rm -f
STRIP = strip

# 头文件路径
CFLAGS +=  -I../src
# 第三方库
EXTLIBS = 

# 编译选项
CFLAGS += -Wall -O2 -Os -g

# 系统库
LDFLAGS +=  -lc 


# 需要编译的文件 
SRCPATH = ../src
SOURCE += $(SRCPATH)/func.cpp
SOURCE += $(SRCPATH)/main.cpp

EXE_OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

TARGET = ./test

all:$(TARGET)

$(TARGET):$(EXE_OBJS) 
	$(CPP) -o $@ $^  $(EXTLIBS) $(LDFLAGS)

# 
.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(TARGET) $(EXE_OBJS)
