CC = gcc

STD = -std=c99 -pedantic
WARN= -Wall 
OPT= -O -g
INC= ../../inc
LIB= ../../lib

LIBRARY := -lnetwork
LIBRARY := $(LIB) $(LIBRARY)



CFLAGS = ${STD} ${WARN} ${OPT} -I${INC} 


TARGET = redis_timer

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -L${LIBRARY}

clean: 
	rm -rf *.o ${TARGET}
