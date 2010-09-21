# vim: ts=8 sw=8

CC	=gcc -march=i686
CFLAGS	=-Os -Wall -Werror -pedantic -g
LDFLAGS	=-g
LDLIBS	=

all:	backoff

clean:
	${RM} *.o core.* lint tags a.out
	${RM} plot data
	${RM} details-*

distclean clobber: clean
	${RM} backoff

check:	data plot
	gnuplot plot

data plot: backoff
	./backoff
