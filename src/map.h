#ifndef GNUITAR_MAP_H
#define GNUITAR_MAP_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup libgnuitar-map Strongly-typed, Key-value Map
 * The map structure is used by audio drivers and effects to communicate
 * their parameters to client code.
 * It is also used to serialize data, such as effect settings and such, into files.
 * It is also used to serialize requests and responses between clients and servers.
 */

/** The type of a map entry.
 * @ingroup libgnuitar-map
 */

enum GnuitarMapType {
    /** The entry is a string */
    GNUITAR_MAP_TYPE_STRING,
    /** The entry is a 16-bit, signed integer */
    GNUITAR_MAP_TYPE_INT16,
    /** The entry is a 32-bit, signed integer */
    GNUITAR_MAP_TYPE_INT32,
    /** The entry is a 64-bit, signed integer */
    GNUITAR_MAP_TYPE_INT64,
    /** The entry is a 16-bit, unsigned integer */
    GNUITAR_MAP_TYPE_UINT16,
    /** The entry is a 32-bit, unsigned integer */
    GNUITAR_MAP_TYPE_UINT32,
    /** The entry is a 64-bit, unsigned integer */
    GNUITAR_MAP_TYPE_UINT64,
    /** The entry is a single precision, floating point type */
    GNUITAR_MAP_TYPE_FLOAT,
    /** The entry is a double precision, floating point type */
    GNUITAR_MAP_TYPE_DOUBLE,
    /** The entry is a boolean type */
    GNUITAR_MAP_TYPE_BOOL,
    /** The entry is another map */
    GNUITAR_MAP_TYPE_MAP
};

size_t gnuitar_map_type_size(enum GnuitarMapType type);

/** An entry within a map structure.
 * Contains the key, value and type of the entry.
 * @ingroup libgnuitar-map
 */

struct GnuitarMapEntry {
    /** The key of the entry */
    char *name;
    /** The value of the entry */
    void *data;
    /** Type of the value */
    enum GnuitarMapType type;
};

const char * gnuitar_map_entry_get_name(const struct GnuitarMapEntry *entry);

void * gnuitar_map_entry_get_data(struct GnuitarMapEntry *entry);

enum GnuitarMapType gnuitar_map_entry_get_type(const struct GnuitarMapEntry *entry);

/** A strongly-typed, key-value map.
 * @ingroup libgnuitar-map
 */

struct GnuitarMap {
    /** The array of entries in the map */
    struct GnuitarMapEntry *entries;
    /** The number of entries in the map */
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

