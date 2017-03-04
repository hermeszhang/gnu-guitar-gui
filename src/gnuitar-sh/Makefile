CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -Wfatal-errors -std=c++14
LDLIBS = -lpthread -ldl

ifdef gnuitar_config
include $(gnuitar_config)
else
gnuitar_with_alsa = 1
endif

ifdef gnuitar_with_alsa
LDLIBS += -lasound
endif

objfiles += gnuitar-sh.o
objfiles += shell.o

.PHONY: all
all: gnuitar-sh

gnuitar-sh: $(objfiles) ../libgnuitar.so
	$(CXX) $(objfiles) ../libgnuitar.so -o $@ $(LDLIBS)

gnuitar-sh.o: gnuitar-sh.cc shell.h ../libgnuitar.so

shell.o: shell.cc shell.h ../libgnuitar.so

.PHONY: clean
clean:
	$(RM) gnuitar-sh
	$(RM) $(objfiles)

