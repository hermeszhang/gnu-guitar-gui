CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors

objfiles += effect.o
objfiles += pipe.o
objfiles += map.o
objfiles += track.o

tests += pipe-test
tests += map-test

ifdef gnuitar_config
include $(gnuitar_config)
else
gnuitar_with_alsa = 1
endif

ifdef gnuitar_with_alsa
CFLAGS += -DGNUITAR_WITH_ALSA
endif

.PHONY: all
all: libgnuitar.a $(tests)

libgnuitar.a: $(objfiles)
	$(AR) $(ARFLAGS) $@ $(objfiles)

effect.o: effect.c effect.h packet.h

pipe.o: pipe.c pipe.h

pipe-test: pipe-test.c libgnuitar.a

map.o: map.c map.h

map-test: map-test.c libgnuitar.a

track.o: track.c track.h packet.h pump.h map.h

.PHONY: clean
clean:
	$(RM) libgnuitar.a $(objfiles) $(tests)

