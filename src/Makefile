CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors

objfiles += pipe.o
objfiles += map.o

.PHONY: all
all: libgnuitar.a

libgnuitar.a: $(objfiles)
	$(AR) $(ARFLAGS) $@ $(objfiles)

pipe.o: pipe.c pipe.h

map.o: map.c map.h

.PHONY: clean
clean:
	$(RM) libgnuitar.a $(objfiles)

