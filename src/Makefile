CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors -ansi -pedantic

objfiles += pipe.o

.PHONY: all
all: libgnuitar.a

libgnuitar.a: $(objfiles)
	$(AR) $(ARFLAGS) $@ $(objfiles)

pipe.o: pipe.c pipe.h

.PHONY: clean
clean:
	$(RM) libgnuitar.a $(objfiles)

