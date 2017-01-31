#include "map.h"

#include <assert.h>
#include <string.h>

int
main(void)
{
    gnuitar_error_t error;
    gnuitar_map_t map;

    gnuitar_map_init(&map);

    error = gnuitar_map_define(&map, "Volume", GNUITAR_MAP_TYPE_FLOAT);
    assert(error == GNUITAR_ERROR_NONE);
    assert(map.entries_count == 1);
    assert(map.entries[0].type == GNUITAR_MAP_TYPE_FLOAT);
    assert(strcmp(map.entries[0].name, "Volume") == 0);

    error = gnuitar_map_define(&map, "Volume", GNUITAR_MAP_TYPE_UINT32);
    assert(error == GNUITAR_ERROR_EEXISTS);
    assert(map.entries_count == 1);
    assert(map.entries[0].type == GNUITAR_MAP_TYPE_FLOAT);
    assert(strcmp(map.entries[0].name, "Volume") == 0);

    error = gnuitar_map_define(&map, "Distortion", GNUITAR_MAP_TYPE_UINT32);
    assert(error == GNUITAR_ERROR_NONE);
    assert(map.entries_count == 2);
    assert(map.entries[0].type == GNUITAR_MAP_TYPE_UINT32);
    assert(strcmp(map.entries[0].name, "Distortion") == 0);
    assert(map.entries[1].type == GNUITAR_MAP_TYPE_FLOAT);
    assert(strcmp(map.entries[1].name, "Volume") == 0);

    error = gnuitar_map_define(&map, "Low Pass", GNUITAR_MAP_TYPE_DOUBLE);
    assert(error == GNUITAR_ERROR_NONE);
    assert(map.entries_count == 3);
    assert(map.entries[0].type == GNUITAR_MAP_TYPE_UINT32);
    assert(strcmp(map.entries[0].name, "Distortion") == 0);
    assert(map.entries[1].type == GNUITAR_MAP_TYPE_DOUBLE);
    assert(strcmp(map.entries[1].name, "Low Pass") == 0);
    assert(map.entries[2].type == GNUITAR_MAP_TYPE_FLOAT);
    assert(strcmp(map.entries[2].name, "Volume") == 0);

    gnuitar_map_done(&map);
    return 0;
}

