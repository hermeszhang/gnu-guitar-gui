CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors
LDLIBS = -lpthread

ifdef gnuitar_config
include $(gnuitar_config)
else
gnuitar_with_alsa = 1
endif

ifdef gnuitar_with_alsa
LDLIBS += -lasound
endif

objfiles += gnuitar-sh.o
objfiles += string.o
objfiles += shell.o

.PHONY: all
all: gnuitar-sh

gnuitar-sh: $(objfiles) ../libgnuitar.so

gnuitar-sh.o: gnuitar-sh.c shell.h ../libgnuitar.so

shell.o: shell.c shell.h string.h

string.o: string.c string.h

.PHONY: clean
clean:
	$(RM) gnuitar-sh
	$(RM) $(objfiles)

