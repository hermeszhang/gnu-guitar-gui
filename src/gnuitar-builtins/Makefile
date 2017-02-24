CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors -fPIC

objfiles += package.o
objfiles += echo.o

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

.PHONY: all
all: libgnuitar-builtins.so $(tests)

libgnuitar-builtins.so: $(objfiles)
	$(CC) -shared $(objfiles) -o $@

package.o: package.c ../gnuitar.h ../package.h ../effect.h ../track.h

package-test: package-test.c ../libgnuitar.so libgnuitar-builtins.so
	$(CC) $(CFLAGS) package-test.c ../libgnuitar.so -lasound -o $@

echo.o: echo.c echo.h ../gnuitar.h ../packet.h

echo-test: echo-test.c echo.o ../libgnuitar.so

.PHONY: clean
clean:
	$(RM) libgnuitar-builtins.so
	$(RM) $(objfiles)
	$(RM) $(tests)

.PHONY: test
test:
	$(VALGRIND) ./echo-test

