CC = gcc
CFLAGS  += `pkg-config --cflags glib-2.0 gthread-2.0`
LDFLAGS += `pkg-config --libs glib-2.0 gthread-2.0`

all:
	$(CC) ${CFLAGS}  test.c -o test ${LDFLAGS}

clean:
	rm -f loop ulti memchunk context

run:
#	G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --leak-check=full ./test
	G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --leak-resolution=high --num-callers=20 --show-reachable=yes --log-file=a.log  ./test