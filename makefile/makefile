
# makefile sample
# 编译静态库

# 工具链
CPP = g++
CC = gcc
LD = ld
AR = ar
RM = rm -f
STRIP = strip

# 头文件路径
CFLAGS += -I../redis-3.0-network/inc

# 编译选项
CFLAGS += -Wall -O2 -Os -g

LDFLAGS += "-wl" -lc 
 
AFLAGS += -r

# 需要编译的文件
SRCPATH = ../redis-3.0-network/src
SOURCE += $(SRCPATH)/zmalloc.c
SOURCE += $(SRCPATH)/sds.c
SOURCE += $(SRCPATH)/endianconv.c
SOURCE += $(SRCPATH)/anet.c
SOURCE += $(SRCPATH)/ae.c
SOURCE += $(SRCPATH)/ae_epoll.c

LIB_OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

LIB_TARGET = ./libnet.a


all:$(LIB_TARGET)

$(LIB_TARGET):$(LIB_OBJS)
	$(RM) $@;
	$(AR) $(AFLAGS) $@ $^

.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(LIB_TARGET) $(LIB_OBJS)
