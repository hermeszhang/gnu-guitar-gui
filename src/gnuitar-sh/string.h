#ifndef GNUITAR_STRING_H
#define GNUITAR_STRING_H

/* for size_t */
#include <stdlib.h>

struct GnuitarString {
    char *buf;
    size_t len;
    size_t res;
};

void gnuitar_string_init(struct GnuitarString *string);

void gnuitar_string_done(struct GnuitarString *string);

int gnuitar_string_add_char(struct GnuitarString *string, char c);

int gnuitar_string_cmp_literal(const struct GnuitarString *a, const char * b);

#endif /* GNUITAR_STRING_H */

