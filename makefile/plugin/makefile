# 工具链
CPP = g++
CC = gcc
RM = rm -f
STRIP = strip

# 头文件路径
CFLAGS += -I ../src

# 第三方库
#EXTLIBS = -L./ -lfunc

# 编译选项
CFLAGS += -Wall -O2 -Os -g -fPIC

# 系统库
LDFLAGS +=  -lc -ldl

# 需要编译的文件（静态库）
SRCPATH = ../src
SOURCE += $(SRCPATH)/func.cpp
LIB_OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

# 需要编译的文件（可执行程序）
SRC += ./main.cpp
EXE_OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC)))

LIB_TARGET = ./libfunc.so

TARGET = ./test

all:$(LIB_TARGET) $(TARGET)

lib:$(LIB_TARGET)

$(LIB_TARGET):$(LIB_OBJS)
	$(CPP) -shared -o $@ $(LIB_OBJS) $(LDFLAGS)   


$(TARGET):$(EXE_OBJS) 
	$(CPP) -o $@ $^  $(EXTLIBS) $(LDFLAGS) 
# 当动态库需要使用可执行程序的变量或者函数时，我们需要将可执行程序的符号表导出为全局符号表
# $(CPP) -Wl,--export-dynamic -o $@ $^  $(EXTLIBS) $(LDFLAGS)  

.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(LIB_TARGET) $(LIB_OBJS) $(TARGET) $(EXE_OBJS)
