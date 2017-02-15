CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors

objfiles += pipe.o
objfiles += map.o

tests += pipe-test
tests += map-test

.PHONY: all
all: libgnuitar.a $(tests)

libgnuitar.a: $(objfiles)
	$(AR) $(ARFLAGS) $@ $(objfiles)

pipe.o: pipe.c pipe.h

pipe-test: pipe-test.c libgnuitar.a

map.o: map.c map.h

map-test: map-test.c libgnuitar.a

.PHONY: clean
clean:
	$(RM) libgnuitar.a $(objfiles) $(tests)

