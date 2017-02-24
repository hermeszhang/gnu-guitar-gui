CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors

ifdef gnuitar_release
CFLAGS += -O3
else
CFLAGS += -g
endif

objfiles += chain.o
objfiles += conversion.o
objfiles += effect.o
objfiles += map.o
objfiles += package.o
objfiles += packet.o
objfiles += pipe.o
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
objfiles += track-alsa.o
endif

.PHONY: all
all: libgnuitar.a $(tests)

libgnuitar.a: $(objfiles)
	$(AR) $(ARFLAGS) $@ $(objfiles)

chain.o: chain.c chain.h effect.h

effect.o: effect.c effect.h packet.h

pipe-test: pipe-test.c libgnuitar.a

map.o: map.c map.h

map-test: map-test.c libgnuitar.a

packet.o: packet.c packet.h

package.o: package.c package.h

pipe.o: pipe.c pipe.h

track.o: track.c track.h packet.h chain.h map.h

track-alsa.o: track-alsa.c track-alsa.h track.h packet.h chain.h map.h

utils.o: utils.c utils.h

.PHONY: clean
clean:
	$(RM) libgnuitar.a $(objfiles) $(tests)

.PHONY: doc
doc:
	doxygen

