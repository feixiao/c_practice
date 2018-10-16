LO_OBJS = foo.lo 
PACKAGE_VERSION = 1:1:1
LIBDIR=/usr/local/lib
BINDIR=/usr/local/bin

all : hello
 
install : hello
	libtool --mode=install install -c libfoo.la ${LIBDIR}/libfoo.la
	libtool --mode=install cp hello ${BINDIR}/hello
 
uninstall : ${LIBDIR}/libfoo.la ${BINDIR}/hello
	libtool --mode=uninstall /bin/rm ${LIBDIR}/libfoo.la
	libtool --mode=uninstall /bin/rm ${BINDIR}/hello
 
hello : libfoo.la hello.lo	
	libtool --mode=link gcc -g -O -o hello hello.lo -rpath ${LIBDIR} libfoo.la

libfoo.la : $(LO_OBJS)
	libtool --mode=link --tag=CC gcc -g -O -o libfoo.la -rpath ${LIBDIR} $(LO_OBJS)

foo.lo : foo.c
	libtool --mode=compile gcc -g -O -c foo.c
 
hello.lo : hello.c
	libtool --mode=compile gcc -g -O -c hello.c
 
clean :
	rm -f lib*.a *~ *core *.lo *.o *.la hello
	rm -rf .libs