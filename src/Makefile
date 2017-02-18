CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors

objfiles += effect.o
objfiles += pipe.o
objfiles += map.o
objfiles += packet.o
objfiles += chain.o
objfiles += track.o
objfiles += utils.o

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

objfiles += track-alsa.o

.PHONY: all
all: libgnuitar.a $(tests)

libgnuitar.a: $(objfiles)
	$(AR) $(ARFLAGS) $@ $(objfiles)

chain.o: chain.c chain.h effect.h

effect.o: effect.c effect.h packet.h

pipe.o: pipe.c pipe.h

pipe-test: pipe-test.c libgnuitar.a

map.o: map.c map.h

map-test: map-test.c libgnuitar.a

packet.o: packet.c packet.h

track.o: track.c track.h packet.h chain.h map.h

track-alsa.o: track-alsa.c track-alsa.h track.h packet.h chain.h map.h

utils.o: utils.c utils.h

.PHONY: clean
clean:
	$(RM) libgnuitar.a $(objfiles) $(tests)

.PHONY: doc
doc:
	doxygen

