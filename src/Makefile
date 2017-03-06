CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -Wfatal-errors -std=c++11

CXXFLAGS += -I../include

VPATH += ../include/gnuitar

ifdef gnuitar_config
include $(gnuitar_config)
else
gnuitar_with_alsa = 1
endif

ifdef gnuitar_release
CXXFLAGS += -O3
else
CXXFLAGS += -g -DGNUITAR_DEBUG
endif

ifdef gnuitar_with_alsa
objfiles += alsa-driver.o
CXXFLAGS += -DGNUITAR_WITH_ALSA=1
LDLIBS += -lasound
endif

LDLIBS += -ldl
LDLIBS += -lpthread

objfiles += plugin.o
objfiles += driver.o

tests += plugin-test
tests += driver-test

.PHONY: all
all: libgnuitar.a $(tests)

libgnuitar.a: $(objfiles)
	$(AR) $(ARFLAGS) $@ $(objfiles)

alsa-driver.o: alsa-driver.cc alsa-driver.h driver.h plugin.h

driver.o: driver.cc driver.h plugin.h

driver-test: driver-test.cc libgnuitar.a

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

