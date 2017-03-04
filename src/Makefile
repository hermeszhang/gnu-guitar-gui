CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors

ifdef gnuitar_release
CFLAGS += -fPIC -O3
else
CFLAGS += -fPIC -g
endif

LDLIBS += -ldl

objfiles += chain.o
objfiles += conversion.o
objfiles += driver.o
objfiles += effect.o
objfiles += env.o
objfiles += map.o
objfiles += package.o
objfiles += package-manager.o
objfiles += packet.o
objfiles += pipe.o
objfiles += track.o
objfiles += mutex.o

tests += pipe-test
tests += map-test

.PHONY: all
all: libgnuitar.so $(tests)

libgnuitar.so: $(objfiles)
	$(CC) -shared $(objfiles) -o $@

chain.o: chain.c chain.h effect.h

driver.o: driver.c driver.h

effect.o: effect.c effect.h packet.h

env.o: env.c env.h

pipe-test: pipe-test.c libgnuitar.so

map.o: map.c map.h

map-test: map-test.c libgnuitar.so

packet.o: packet.c packet.h

package.o: package.c package.h

package-manager.o: package-manager.c package-manager.h package.h

pipe.o: pipe.c pipe.h

track.o: track.c track.h packet.h chain.h map.h

mutex.o: mutex.c mutex.h

.PHONY: clean
clean:
	$(RM) libgnuitar.so $(objfiles) $(tests)
	$(MAKE) -C gnuitar-builtins clean
	$(MAKE) -C gnuitar-sh clean

.PHONY: doc
doc:
	doxygen

