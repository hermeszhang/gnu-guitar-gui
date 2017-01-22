#ifndef GNUITAR_MAP_H
#define GNUITAR_MAP_H

#include "error.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum gnuitar_map_type {
    GNUITAR_MAP_TYPE_STRING,
    GNUITAR_MAP_TYPE_INT,
    GNUITAR_MAP_TYPE_UINT,
    GNUITAR_MAP_TYPE_REAL,
    GNUITAR_MAP_TYPE_BOOL
} gnuitar_map_type_t;

size_t gnuitar_map_type_size(gnuitar_map_type_t type);

typedef struct gnuitar_map_entry {
    char *name;
    void *data;
    gnuitar_map_type_t type;
} gnuitar_map_entry_t;

const char * gnuitar_map_entry_get_name(const gnuitar_map_entry_t *entry);

void * gnuitar_map_entry_get_data(gnuitar_map_entry_t *entry);

gnuitar_map_type_t gnuitar_map_entry_get_type(const gnuitar_map_entry_t *entry);

typedef struct gnuitar_map {
    gnuitar_map_entry_t *entries;
    size_t entries_count;
} gnuitar_map_t;

void gnuitar_map_init(gnuitar_map_t *map);

void gnuitar_map_done(gnuitar_map_t *map);

gnuitar_error_t gnuitar_map_define(gnuitar_map_t *map, const char *name, gnuitar_map_type_t type);

unsigned char gnuitar_map_exists(const gnuitar_map_t *map, const char *name);

gnuitar_map_entry_t * gnuitar_map_find(const gnuitar_map_t *map, const char *name);

gnuitar_error_t gnuitar_map_set(gnuitar_map_t *map, const char *name, const void *data);

gnuitar_error_t gnuitar_map_get(gnuitar_map_t *map, const char *name, void *data);

size_t gnuitar_map_get_count(const gnuitar_map_t *map);

const char * gnuitar_map_get_name(const gnuitar_map_t *map, size_t i);

gnuitar_error_t gnuitar_map_get_type(const gnuitar_map_t *map, const char *name, gnuitar_map_type_t *type);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_MAP_H */

