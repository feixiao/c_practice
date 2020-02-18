# 工具链
CPP = g++
CC = gcc
LD = ld
AR = ar
RM = rm -f
STRIP = strip

OSNAME    = $(shell uname -s)
OSTYPE    = $(shell uname -m)

RPATH =

# 头文件路径
CFLAGS += -I ./
CFLAGS += -I ../
CFLAGS += -I ../../input/inc/zookeeper

# 编译选项
CFLAGS += -Wall -O2 -Os -g

# 系统库
LDFLAGS +=  -lc 

# $(AR) $(AFLAGS) ar -r 生成静态库的命令
AFLAGS += -r

# 需要编译的文件（静态库）
SRCPATH = .
SOURCE += $(SRCPATH)/service.cpp
LIB_OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))


ifeq ($(findstring Linux, $(OSNAME)), Linux)
    ifeq ($(findstring i686, $(OSTYPE)), i686)
		RPATH = linux32
	endif

    ifeq ($(findstring x86_64, $(OSTYPE)), x86_64)
		RPATH = linux64
	endif

	n = `cat /proc/cpuinfo | grep processor | wc -l`
	MAKE_ARGS = -j $(n)
	SYSLIB += -lrt -ldl
endif

# For Darwin
ifeq ($(findstring Darwin, $(OSNAME)), Darwin)
	RPATH = macos
	SYSLIB +=  -rdynamic -L/usr/lib -liconv
	LDFLAGS = -dynamiclib -shared
endif

LIB_TARGET = ../../dist/${RPATH}/libcluster.a

all:$(LIB_TARGET) 

lib:$(LIB_TARGET)

# $@: 表示目标文件，编译器会自动将这个变量扩展为当前规则的目标文件名
# $^: 表示所有依赖文件，编译器会自动将这个变量扩展为当前规则所有依赖的文件
$(LIB_TARGET):$(LIB_OBJS)
	$(AR) $(AFLAGS) $@ $^
 
.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	@echo $(OSNAME)
	$(RM) $(LIB_TARGET) $(LIB_OBJS)
