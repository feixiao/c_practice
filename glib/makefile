CC = gcc
CFLAGS  += `pkg-config --cflags glib-2.0 gthread-2.0`
LDFLAGS += `pkg-config --libs glib-2.0 gthread-2.0`

all:
	$(CC) ${CFLAGS}  loop.c -o loop ${LDFLAGS}
	$(CC) ${CFLAGS}  ulti.c -o ulti ${LDFLAGS}
	$(CC) ${CFLAGS}  memchunk.c -o memchunk ${LDFLAGS}
	$(CC) ${CFLAGS}  context.c -o context ${LDFLAGS}

clean:
	rm -f loop ulti memchunk context