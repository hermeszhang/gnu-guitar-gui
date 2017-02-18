#include "map.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#ifdef _MSC_VER
#define gnuitar_strdup _strdup
#else /* _MSC_VER */
#define gnuitar_strdup strdup
#endif /* _MSC_VER */

static void
gnuitar_map_entry_done(struct GnuitarMapEntry *entry)
{
    if (entry != NULL) {
        free(entry->name);
        if (entry->type == GNUITAR_MAP_TYPE_MAP)
            gnuitar_map_entry_done((struct GnuitarMapEntry *)(entry->data));
        free(entry->data);
    }
}

static int
gnuitar_map_entry_compare(const void *a_ptr, const void *b_ptr)
{
    const struct GnuitarMapEntry *a = (const struct GnuitarMapEntry *)(a_ptr);
    const struct GnuitarMapEntry *b = (const struct GnuitarMapEntry *)(b_ptr);
    return strcmp(a->name, b->name);
}

static int
gnuitar_map_name_compare(const void *a_ptr, const void *b_ptr)
{
    const char * a = (const char *)(a_ptr);
    const struct GnuitarMapEntry *b = (const struct GnuitarMapEntry *)(b_ptr);
    return strcmp(a, b->name);
}

/** Returns the size of the type, in bytes.
 * @param type The type in question.
 * @returns The size of the type.
 * @ingroup libgnuitar-map
 */

size_t
gnuitar_map_type_size(enum GnuitarMapType type)
{
    switch (type) {
    case GNUITAR_MAP_TYPE_STRING:
        return sizeof(char*);
    case GNUITAR_MAP_TYPE_INT16:
        return sizeof(int16_t);
    case GNUITAR_MAP_TYPE_INT32:
        return sizeof(int32_t);
    case GNUITAR_MAP_TYPE_INT64:
        return sizeof(int64_t);
    case GNUITAR_MAP_TYPE_UINT16:
        return sizeof(uint16_t);
    case GNUITAR_MAP_TYPE_UINT32:
        return sizeof(uint32_t);
    case GNUITAR_MAP_TYPE_UINT64:
        return sizeof(uint64_t);
    case GNUITAR_MAP_TYPE_FLOAT:
        return sizeof(float);
    case GNUITAR_MAP_TYPE_DOUBLE:
        return sizeof(double);
    case GNUITAR_MAP_TYPE_BOOL:
        return sizeof(unsigned char);
    case GNUITAR_MAP_TYPE_MAP:
        return sizeof(struct GnuitarMap);
    }
    return 0;
}

/** Initializes a map structure.
 * @param map An unitialized map structure.
 * @ingroup libgnuitar-map
 */

void
gnuitar_map_init(struct GnuitarMap *map)
{
    map->entries = NULL;
    map->entries_count = 0;
}

/** Releases resources allocated by map.
 * @param map An initialized map structure.
 * @ingroup libgnuitar-map
 */
void
gnuitar_map_done(struct GnuitarMap *map)
{
    if (map != NULL){
        unsigned int i;
        for (i = 0; i < map->entries_count; i++)
            gnuitar_map_entry_done(&map->entries[i]);
        free(map->entries);
    }
}

/** Defines a new entry in the map.
 * This function fails if the entry already exists.
 * @param map An initialized map structure.
 * @param name The name of the entry
 * @param type The type of the entry
 * @returns On success, zero is returned.
 *  If the entry already exists, EEXIST is returned.
 *  If a memory allocation fails, ENOMEM is returned.
 * @ingroup libgnuitar-map
 */

int
gnuitar_map_define(struct GnuitarMap *map, const char *name, enum GnuitarMapType type)
{
    struct GnuitarMapEntry *tmp_entries;
    struct GnuitarMapEntry tmp_entry;
    size_t entry_size;
    size_t entries_size;

    if (gnuitar_map_exists(map, name))
        return EEXIST;

    tmp_entry.type = type;

    tmp_entry.name = gnuitar_strdup(name);
    if (tmp_entry.name == NULL)
        return ENOMEM;

    entry_size = gnuitar_map_type_size(type);

    tmp_entry.data = malloc(entry_size);
    if (tmp_entry.data == NULL) {
        free(tmp_entry.name);
	return ENOMEM;
    } else if (type == GNUITAR_MAP_TYPE_MAP) {
        gnuitar_map_init((struct GnuitarMap *)(tmp_entry.data));
    }

    entries_size = map->entries_count + 1;
    entries_size *= sizeof(struct GnuitarMapEntry);

    tmp_entries = realloc(map->entries, entries_size);
    if (tmp_entries == NULL) {
        free(tmp_entry.name);
        free(tmp_entry.data);
	return ENOMEM;
    }

    tmp_entries[map->entries_count] = tmp_entry;

    map->entries = tmp_entries;
    map->entries_count++;

    qsort(map->entries, map->entries_count, sizeof(struct GnuitarMapEntry), gnuitar_map_entry_compare);

    return 0;
}

/** Checks to see if the entry exists.
 * @param map An initialized map
 * @param name The name of the entry
 * @returns If the entry is found, non-zero is returned.
 *  If the entry is not found, zero is returned.
 * @ingroup libgnuitar-map
 */

unsigned char
gnuitar_map_exists(const struct GnuitarMap *map, const char *name)
{
    if (gnuitar_map_find(map, name) == NULL)
        return 0;
    return 1;
}

/** Finds an entry.
 * @param map An initialized map
 * @param name The name of the entry
 * @returns A pointer to the entry, if it is found.
 *  If it is not found, NULL is returned.
 * @ingroup libgnuitar-map
 */

struct GnuitarMapEntry *
gnuitar_map_find(const struct GnuitarMap *map, const char *name)
{
    void * result;
    result = bsearch(name,
                     map->entries,
                     map->entries_count,
                     sizeof(*map->entries),
                     gnuitar_map_name_compare);
    return (struct GnuitarMapEntry *)(result);
}

/** Returns the number of entries in the map.
 * @param map An initialized map
 * @returns The number of entries in the map.
 * @ingroup libgnuitar-map
 */

size_t
gnuitar_map_get_count(const struct GnuitarMap *map)
{
    return map->entries_count;
}

/** Returns the name of a entry by index.
 * The function fails if the index is out of boundaries.
 * @param map An initialized map
 * @param index The index of the entry
 * @returns On success, the name of the entry is returned.
 *  On failure, NULL is returned.
 * @ingroup libgnuitar-map
 */

const char *
gnuitar_map_get_name(const struct GnuitarMap *map, size_t index)
{
    if (index >= map->entries_count)
        return NULL;
    return map->entries[index].name;
}

