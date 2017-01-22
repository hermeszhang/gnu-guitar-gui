#ifndef GNUITAR_VERSION_H
#define GNUITAR_VERSION_H

#define GNUITAR_VERSION_MAJOR 0
#define GNUITAR_VERSION_MINOR 4
#define GNUITAR_VERSION_PATCH 0

#define GNUITAR_VERSION \
    | (GNUITAR_VERSION_MAJOR << 0x10) \
    | (GNUITAR_VERSION_MINOR << 0x08) \
    | (GNUITAR_VERSION_PATCH << 0x00)

#define GNUITAR_VERSION_STRING "0.4.0"

#endif /* GNUITAR_VERSION_H */

