#ifndef GNUITAR_DRIVER_H
#define GNUITAR_DRIVER_H

#include "chain.h"
#include "map.h"

struct GnuitarDriver {
    void *data;
    void (*done)(void *data);
    int (*start)(void *data, struct GnuitarChain *chain);
    int (*stop)(void *data);
    int (*get_map)(const void *data, struct GnuitarMap *map);
    int (*set_map)(void *data, const struct GnuitarMap *map);
};

void gnuitar_driver_init(struct GnuitarDriver *driver);

void gnuitar_driver_done(struct GnuitarDriver *driver);

int gnuitar_driver_start(struct GnuitarDriver *driver, struct GnuitarChain *chain);

int gnuitar_driver_stop(struct GnuitarDriver *driver);

int gnuitar_driver_get_map(const struct GnuitarDriver *driver, struct GnuitarMap *map);

int gnuitar_driver_set_map(struct GnuitarDriver *driver, const struct GnuitarMap *map);

#endif /* GNUITAR_DRIVER_H */

