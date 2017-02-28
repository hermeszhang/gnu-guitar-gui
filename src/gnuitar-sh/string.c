#include "string.h"

#include <string.h>

#include <errno.h>

void
gnuitar_string_init(struct GnuitarString *string)
{
    string->buf = NULL;
    string->len = 0;
    string->res = 0;
}

void
gnuitar_string_done(struct GnuitarString *string)
{
    free(string->buf);
    string->buf = NULL;
    string->len = 0;
    string->res = 0;
}

int
gnuitar_string_add_char(struct GnuitarString *string, char c)
{
    char *tmp;

    if ((string->len + 2) > string->res){
        tmp = realloc(string->buf, string->res + 32);
        if (tmp == NULL)
            return ENOMEM;
        string->buf = tmp;
        string->res += 32;
    }

    string->buf[string->len + 0] = c;
    string->buf[string->len + 1] = 0;
    string->len++;

    return 0;
}

int
gnuitar_string_cmp_literal(const struct GnuitarString *a, const char *b)
{
    return strcmp(a->buf, b);
}

