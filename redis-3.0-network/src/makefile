PREFIX=..
DESTDIR=
LIBDIR=${PREFIX}/lib
INCDIR=${PREFIX}/inc

GCC	:= gcc
LD	:= ld
AR	:= ar

LDFLAGS	:= -shared -fpic
ARFLAGS	:= -rc
CFLAGS	:= -g -Wall -fpic

DIR_INC	:= .
DIR_SRC	:= .
DIR_OBJ	:= .
DIR_LIB	:= .

LIB_NAME	:= libnetwork

SRC	:= $(wildcard ${DIR_SRC}/*.c)
OBJ	:= $(patsubst %.c, ${DIR_OBJ}/%.o, $(notdir ${SRC}))

SHARE_LIB	:= $(LIB_NAME).so
DYNAMIC_LIB	:= $(LIB_NAME).a

INC	:= -I${DIR_INC}
 
all:$(OBJ)
	#$(LD) $(LDFLAGS) -o $(DIR_LIB)/$(SHARE_LIB) $(OBJ)
	${AR} ${ARFLAGS} $(DIR_LIB)/$(DYNAMIC_LIB) $(OBJ) 
	#${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	@echo Compiling $(OBJ) ...
	$(GCC) $(CFLAGS) -o $@ -c $< ${INC} -g

clean:
	rm -rf $(DIR_OBJ)/*.o $(DIR_LIB)/*.so $(DIR_LIB)/*.a


install:
	install -m 755 -d ${DESTDIR}${INCDIR}
	install -m 755 -d ${DESTDIR}${LIBDIR}
	install -m 755 libnetwork.a ${DESTDIR}${LIBDIR}/libnetwork.a
	install -m 755 ae.h ${DESTDIR}${INCDIR}/ae.h
