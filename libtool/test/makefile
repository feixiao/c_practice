LO_OBJS = foo.lo 
PACKAGE_VERSION = 1:1:1
LIBDIR=/usr/local/lib
BINDIR=/usr/local/bin

all : hello
 
install : libhello.la hello
	libtool --mode=install install -c libhello.la ${LIBDIR}/libhello.la
	libtool --mode=install cp hello ${BINDIR}/hello
 
uninstall : ${LIBDIR}/libhello.la ${BINDIR}/hello
	libtool --mode=uninstall /bin/rm ${LIBDIR}/libhello.la
	libtool --mode=uninstall /bin/rm ${BINDIR}/hello
 
hello : libhello.la hello.o	
	libtool --mode=link gcc -g -O -o hello hello.lo -rpath ${LIBDIR} libhello.la

libhello.la : $(LO_OBJS)
#	libtool --mode=link --tag=CC gcc -g -O -o libhello.la -rpath ${LIBDIR} ${PACKAGE_VERSION}  $(LO_OBJS)
	libtool --mode=link --tag=CC gcc -g -O -o libhello.la -rpath /usr/local/lib foo.lo

foo.lo : foo.c
	libtool --mode=compile gcc -g -O -c foo.c
 
hello.lo : hello.c
	libtool --mode=compile gcc -g -O -c hello.c
 
clean :
	rm -f lib*.a *~ *core *.lo *.o *.la hello
	rm -rf .libs