# 工具链
CPP = g++
CC = gcc
LD = ld
AR = ar
RM = rm -f
STRIP = strip

# 头文件路径
CFLAGS += -I ../src

# 第三方库
EXTLIBS = -L./ -lfunc

# 编译选项
CFLAGS += -Wall -O2 -Os -g

# 系统库
LDFLAGS +=  -lc 

# $(AR) $(AFLAGS) ar -r 生成静态库的命令
AFLAGS += -r

# 需要编译的文件（静态库）
SRCPATH = ../src
SOURCE += $(SRCPATH)/func.cpp
LIB_OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

# 需要编译的文件（可执行程序）
SRC += $(SRCPATH)/main.cpp
EXE_OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC)))

LIB_TARGET = ./libfunc.a

TARGET = ./test

all:$(LIB_TARGET) $(TARGET)

lib:$(LIB_TARGET)

# $@: 表示目标文件，编译器会自动将这个变量扩展为当前规则的目标文件名
# $^: 表示所有依赖文件，编译器会自动将这个变量扩展为当前规则所有依赖的文件
$(LIB_TARGET):$(LIB_OBJS)
	$(AR) $(AFLAGS) $@ $^


$(TARGET):$(EXE_OBJS) 
	$(CPP) -o $@ $^  $(EXTLIBS) $(LDFLAGS) 

# 
.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(LIB_TARGET) $(LIB_OBJS) $(TARGET) $(EXE_OBJS)
