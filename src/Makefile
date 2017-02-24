CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors

ifdef gnuitar_release
CFLAGS += -fPIC -O3
else
CFLAGS += -fPIC -g
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
LDLIBS += -lasound
objfiles += track-alsa.o
endif

.PHONY: all
all: libgnuitar.so $(tests)

libgnuitar.so: $(objfiles)
	$(CC) -shared $(objfiles) -o $@

chain.o: chain.c chain.h effect.h

effect.o: effect.c effect.h packet.h

pipe-test: pipe-test.c libgnuitar.so

map.o: map.c map.h

map-test: map-test.c libgnuitar.so

packet.o: packet.c packet.h

package.o: package.c package.h

pipe.o: pipe.c pipe.h

track.o: track.c track.h packet.h chain.h map.h

track-alsa.o: track-alsa.c track-alsa.h track.h packet.h chain.h map.h

utils.o: utils.c utils.h

.PHONY: clean
clean:
	$(RM) libgnuitar.so $(objfiles) $(tests)
	$(MAKE) -C gnuitar-builtins clean
	$(MAKE) -C gnuitar-sh clean

.PHONY: doc
doc:
	doxygen

