#ifndef GNUITAR_ERROR_H
#define GNUITAR_ERROR_H

typedef enum gnuitar_error {
    GNUITAR_ERROR_NONE = 0,
    GNUITAR_ERROR_UNKNOWN,
    GNUITAR_ERROR_MALLOC,
    GNUITAR_ERROR_ENOENT,
    GNUITAR_ERROR_EEXISTS
} gnuitar_error_t;

const char * gnuitar_strerror(gnuitar_error_t error);

#endif /* GNUITAR_ERROR_H */

