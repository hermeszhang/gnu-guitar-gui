#include "echo.h"

#include <assert.h>

static void
test_get_and_set_map(void)
{

    struct GnuitarEffect effect;
    struct GnuitarMap effect_map;
    size_t delay;
    size_t delay_out;
    size_t echo_count;
    size_t echo_count_out;

    gnuitar_map_init(&effect_map);

    assert(gnuitar_echo_init(&effect) == 0);
    assert(gnuitar_echo_get_map(&effect, &effect_map) == 0);
    assert(gnuitar_map_get_count(&effect_map) == 2);

    delay = 500; /* 500 milliseconds */
    assert(gnuitar_map_set(&effect_map, "Delay", &delay) == 0);

    assert(gnuitar_map_get(&effect_map, "Delay", &delay_out) == 0);
    assert(delay_out == 500);

    echo_count = 4;
    assert(gnuitar_map_set(&effect_map, "Echo Count", &echo_count) == 0);

    assert(gnuitar_map_get(&effect_map, "Echo Count", &echo_count_out) == 0);
    assert(echo_count_out == 4);

    gnuitar_map_done(&effect_map);

    gnuitar_effect_done(&effect);
}

static void
test_process(void)
{
    struct GnuitarEffect effect;
    struct GnuitarPacket packet;

    assert(gnuitar_echo_init(&effect) == 0);

    gnuitar_packet_init(&packet);

    gnuitar_packet_done(&packet);

    gnuitar_effect_done(&effect);
}

int
main(void)
{
    test_get_and_set_map();
    test_process();
    return 0;
}

