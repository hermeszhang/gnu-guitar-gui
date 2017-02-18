#include "../gnuitar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct GnuitarString {
    char *buf;
    size_t len;
};

static void
gnuitar_string_init(struct GnuitarString *string)
{
    string->buf = NULL;
    string->len = 0;
}

static void
gnuitar_string_done(struct GnuitarString *string)
{
    free(string->buf);
    string->buf = NULL;
    string->len = 0;
}

static int
gnuitar_string_add_char(struct GnuitarString *string, char c)
{
    char *tmp;

    tmp = realloc(string->buf, string->len + 2);
    if (tmp == NULL)
        return ENOMEM;

    string->buf = tmp;
    string->buf[string->len + 0] = c;
    string->buf[string->len + 1] = 0;
    string->len++;

    return 0;
}

static int
gnuitar_string_cmp_literal(const struct GnuitarString *a, const char *b)
{
    return strcmp(a->buf, b);
}

static int
gnuitar_read_cmd(struct GnuitarString *string)
{
    int c;

    gnuitar_string_init(string);

    for (;;) {
        c = fgetc(stdin);
	if (c == EOF) {
            gnuitar_string_done(string);
	    return -1;
	} else if (c == '\n') {
	    break;
	} else if (gnuitar_string_add_char(string, c) != 0) {
	    gnuitar_string_done(string);
	    return -1;
	}
    }

    return 0;
}

int
main(void)
{
    struct GnuitarString cmd;
    struct GnuitarTrack *track;

    track = gnuitar_track_create("ALSA");
    if (track == NULL) {
        fprintf(stderr, "failed to create track\n");
        return EXIT_FAILURE;
    }

    for (;;) {

        if (gnuitar_read_cmd(&cmd) != 0) {
            fprintf(stderr, "failed to read next command\n");
	    gnuitar_track_destroy(track);
	    return EXIT_FAILURE;
	}

	if (gnuitar_string_cmp_literal(&cmd, "start") == 0) {
            if (gnuitar_track_start(track) != 0) {
	        fprintf(stderr, "failed to start track\n");
	    }
	} else if (gnuitar_string_cmp_literal(&cmd, "stop") == 0) {
            if (gnuitar_track_stop(track) != 0) {
	        fprintf(stderr, "failed to stop track\n");
	    }
	} else if (gnuitar_string_cmp_literal(&cmd, "help") == 0) {
	    printf("available commands are:\n");
	    printf("\thelp\n");
	    printf("\tstart\n");
	    printf("\tstop\n");
	    printf("\tquit\n");
	} else if (gnuitar_string_cmp_literal(&cmd, "quit") == 0) {
	    gnuitar_string_done(&cmd);
	    break;
	} else {
            fprintf(stderr, "unknown command: %s\n", cmd.buf);
	}

	gnuitar_string_done(&cmd);
    }

    gnuitar_track_destroy(track);

    return EXIT_SUCCESS;
}

