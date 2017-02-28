CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors -fPIC

objfiles += biquad.o
objfiles += package.o
objfiles += echo.o
objfiles += tremolo.o
objfiles += vibrato.o

tests += echo-test
tests += package-test

ifdef gnuitar_config
include $(gnuitar_config)
else
gnuitar_with_valgrind = 1
gnuitar_with_alsa = 1
endif

ifdef gnuitar_with_valgrind
VALGRIND = valgrind --quiet --error-exitcode=1
endif

ifdef gnuitar_with_alsa
LDLIBS += -lgnuitar -lasound
LDFLAGS += -L..
endif

effect_headers += ../effect.h
effect_headers += ../packet.h
effect_headers += ../map.h

.PHONY: all
all: libgnuitar-builtins.so $(tests)

libgnuitar-builtins.so: $(objfiles)
	$(CC) -shared $(objfiles) ../libgnuitar.so -o $@

biquad.o: biquad.c biquad.h

package.o: package.c ../gnuitar.h ../package.h ../effect.h ../track.h

package-test: package-test.c ../libgnuitar.so libgnuitar-builtins.so
	$(CC) $(CFLAGS) package-test.c ../libgnuitar.so -lasound -o $@

echo.o: echo.c echo.h $(effect_headers)

echo-test: echo-test.c echo.o ../libgnuitar.so

vibrato.o: vibrato.c vibrato.h biquad.h $(effect_headers)

tremolo.o: tremolo.c tremolo.h $(effect_headers)

.PHONY: clean
clean:
	$(RM) libgnuitar-builtins.so
	$(RM) $(objfiles)
	$(RM) $(tests)

.PHONY: test
test:
	$(VALGRIND) ./echo-test

