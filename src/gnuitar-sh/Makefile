CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors
LDLIBS = -lpthread

ifdef gnuitar_config
include $(gnuitar_config)
else
gnuitar_with_alsa = 1
endif

ifdef gnuitar_with_alsa
LDLIBS += -lasound
endif

.PHONY: all
all: gnuitar-sh

gnuitar-sh: gnuitar-sh.c ../libgnuitar.so

.PHONY: clean
clean:
	$(RM) gnuitar-sh

