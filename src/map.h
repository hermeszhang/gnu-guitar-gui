#ifndef GNUITAR_MAP_H
#define GNUITAR_MAP_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum GnuitarMapType {
    GNUITAR_MAP_TYPE_STRING,
    GNUITAR_MAP_TYPE_INT16,
    GNUITAR_MAP_TYPE_INT32,
    GNUITAR_MAP_TYPE_INT64,
    GNUITAR_MAP_TYPE_UINT16,
    GNUITAR_MAP_TYPE_UINT32,
    GNUITAR_MAP_TYPE_UINT64,
    GNUITAR_MAP_TYPE_FLOAT,
    GNUITAR_MAP_TYPE_DOUBLE,
    GNUITAR_MAP_TYPE_BOOL,
    GNUITAR_MAP_TYPE_MAP
};

size_t gnuitar_map_type_size(enum GnuitarMapType type);

struct GnuitarMapEntry {
    char *name;
    void *data;
    enum GnuitarMapType type;
};

const char * gnuitar_map_entry_get_name(const struct GnuitarMapEntry *entry);

void * gnuitar_map_entry_get_data(struct GnuitarMapEntry *entry);

enum GnuitarMapType gnuitar_map_entry_get_type(const struct GnuitarMapEntry *entry);

struct GnuitarMap {
    struct GnuitarMapEntry *entries;
    size_t entries_count;
};

void gnuitar_map_init(struct GnuitarMap *map);

void gnuitar_map_done(struct GnuitarMap *map);

int gnuitar_map_define(struct GnuitarMap *map, const char *name, enum GnuitarMapType type);

unsigned char gnuitar_map_exists(const struct GnuitarMap *map, const char *name);

struct GnuitarMapEntry * gnuitar_map_find(const struct GnuitarMap *map, const char *name);

int gnuitar_map_set(struct GnuitarMap *map, const char *name, const void *data);

int gnuitar_map_get(struct GnuitarMap *map, const char *name, void *data);

size_t gnuitar_map_get_count(const struct GnuitarMap *map);

const char * gnuitar_map_get_name(const struct GnuitarMap *map, size_t i);

int gnuitar_map_get_type(const struct GnuitarMap *map, const char *name, enum GnuitarMapType *type);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_MAP_H */

