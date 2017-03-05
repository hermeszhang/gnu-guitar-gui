CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -Wfatal-errors -std=c++11

ifdef gnuitar_release
CXXFLAGS += -O3
else
CXXFLAGS += -g -DGNUITAR_DEBUG
endif

LDLIBS += -ldl

objfiles += plugin.o

tests += plugin-test

.PHONY: all
all: libgnuitar.a $(tests)

libgnuitar.a: $(objfiles)
	$(AR) $(ARFLAGS) $@ $(objfiles)

plugin.o: plugin.cc plugin.h

plugin-test: plugin-test.cc libgnuitar.a

.PHONY: clean
clean:
	$(RM) libgnuitar.so $(objfiles) $(tests)
	$(MAKE) -C gnuitar-builtins clean
	$(MAKE) -C gnuitar-sh clean

.PHONY: doc
doc:
	doxygen

