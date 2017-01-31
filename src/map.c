#include "map.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _MSC_VER
#define gnuitar_strdup _strdup
#else /* _MSC_VER */
#define gnuitar_strdup strdup
#endif /* _MSC_VER */

static void
gnuitar_map_entry_done(gnuitar_map_entry_t *entry)
{
    if (entry != NULL) {
        free(entry->name);
        if (entry->type == GNUITAR_MAP_TYPE_MAP)
            gnuitar_map_entry_done((gnuitar_map_entry_t *)(entry->data));
        free(entry->data);
    }
}

static int
gnuitar_map_entry_compare(const void *a_ptr, const void *b_ptr)
{
    const gnuitar_map_entry_t *a = (const gnuitar_map_entry_t *)(a_ptr);
    const gnuitar_map_entry_t *b = (const gnuitar_map_entry_t *)(b_ptr);
    return strcmp(a->name, b->name);
}

static int
gnuitar_map_name_compare(const void *a_ptr, const void *b_ptr)
{
    const char * a = (const char *)(a_ptr);
    const gnuitar_map_entry_t *b = (const gnuitar_map_entry_t *)(b_ptr);
    return strcmp(a, b->name);
}

size_t
gnuitar_map_type_size(gnuitar_map_type_t type)
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
        return sizeof(gnuitar_map_t);
    }
    return 0;
}

void
gnuitar_map_init(gnuitar_map_t *map)
{
    map->entries = NULL;
    map->entries_count = 0;
}

void
gnuitar_map_done(gnuitar_map_t *map)
{
    if (map != NULL){
        unsigned int i;
        for (i = 0; i < map->entries_count; i++)
            gnuitar_map_entry_done(&map->entries[i]);
        free(map->entries);
    }
}

gnuitar_error_t
gnuitar_map_define(gnuitar_map_t *map, const char *name, gnuitar_map_type_t type)
{
    gnuitar_map_entry_t *tmp_entries;
    gnuitar_map_entry_t tmp_entry;
    size_t entry_size;
    size_t entries_size;

    if (gnuitar_map_exists(map, name))
        return GNUITAR_ERROR_EEXISTS;

    tmp_entry.type = type;

    tmp_entry.name = gnuitar_strdup(name);
    if (tmp_entry.name == NULL)
        return GNUITAR_ERROR_MALLOC;

    entry_size = gnuitar_map_type_size(type);

    tmp_entry.data = malloc(entry_size);
    if (tmp_entry.data == NULL) {
        free(tmp_entry.name);
        return GNUITAR_ERROR_MALLOC;
    } else if (type == GNUITAR_MAP_TYPE_MAP) {
        gnuitar_map_init((gnuitar_map_t *)(tmp_entry.data));
    }

    entries_size = map->entries_count + 1;
    entries_size *= sizeof(gnuitar_map_entry_t);

    tmp_entries = realloc(map->entries, entries_size);
    if (tmp_entries == NULL) {
        free(tmp_entry.name);
        free(tmp_entry.data);
        return GNUITAR_ERROR_MALLOC;
    }

    tmp_entries[map->entries_count] = tmp_entry;

    map->entries = tmp_entries;
    map->entries_count++;

    qsort(map->entries, map->entries_count, sizeof(gnuitar_map_entry_t), gnuitar_map_entry_compare);

    return GNUITAR_ERROR_NONE;
}

unsigned char
gnuitar_map_exists(const gnuitar_map_t *map, const char *name)
{
    if (gnuitar_map_find(map, name) == NULL)
        return 0;
    return 1;
}

gnuitar_map_entry_t *
gnuitar_map_find(const gnuitar_map_t *map, const char *name)
{
    void * result;
    result = bsearch(name,
                     map->entries,
                     map->entries_count,
                     sizeof(*map->entries),
                     gnuitar_map_name_compare);
    return (gnuitar_map_entry_t *)(result);
}

size_t
gnuitar_map_get_count(const gnuitar_map_t *map)
{
    return map->entries_count;
}

const char *
gnuitar_map_get_name(const gnuitar_map_t *map, size_t i)
{
    if (i >= map->entries_count)
        return NULL;
    return map->entries[i].name;
}

