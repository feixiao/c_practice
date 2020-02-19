# 编译可执行程序

# 工具链
CPP = g++
CC = gcc
RM = rm -f
STRIP = strip

# 系统信息
OSNAME    = $(shell uname -s)
OSTYPE    = $(shell uname -m)
RPATH =

ifeq ($(findstring Linux, $(OSNAME)), Linux)
    ifeq ($(findstring i686, $(OSTYPE)), i686)
		RPATH = linux32
	endif

    ifeq ($(findstring x86_64, $(OSTYPE)), x86_64)
		RPATH = linux64
	endif

	n = `cat /proc/cpuinfo | grep processor | wc -l`
	MAKE_ARGS = -j $(n)
endif

# For Darwin
ifeq ($(findstring Darwin, $(OSNAME)), Darwin)
	RPATH = macos
endif

# 头文件路径
CFLAGS +=  -I../src
# 第三方库
EXTLIBS = ../dist/${RPATH}/libfunc.a

# 编译选项
CFLAGS += -Wall -O2 -Os -g

# 系统库
LDFLAGS +=  -lc 


# 需要编译的文件 
SRCPATH = ../src
SOURCE += $(SRCPATH)/main.cpp

EXE_OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

TARGET = ../dist/${RPATH}/test

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
