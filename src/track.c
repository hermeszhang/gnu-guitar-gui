/*
 * Audio driver interface to reduce file interdependencies and give home to
 * various state variables. (c) 2006 Antti S. Lankila. Licensed under GPL.
 *
 * $Id$
 *
 * $Log$
 * Revision 1.3  2006/08/10 16:18:36  alankila
 * - improve const correctness and make gnuitar compile cleanly under
 *   increasingly pedantic warning models.
 *
 * Revision 1.2  2006/08/08 21:05:31  alankila
 * - optimize gnuitar: this breaks dsound, I'll fix it later
 *
 * Revision 1.1  2006/08/07 12:55:30  alankila
 * - construct audio-driver.c to hold globals and provide some utility
 *   functions to its users. This slashes interdependencies somewhat.
 *
 */

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "track.h"
#include "track-alsa.h"

#include "audio-oss.h"
#include "audio-jack.h"
#include "audio-winmm.h"
#include "audio-dsound.h"

struct GnuitarTrack *
gnuitar_track_create(const char *name)
{
#ifdef GNUITAR_WITH_ALSA
    if (strcmp(name, "ALSA") == 0) {
        return gnuitar_alsa_track_create();
    }
#endif /* GNUITAR_WITH_ALSA */
    if (strcmp(name, "Null") == 0) {
        /* TODO */
        return NULL;
    }
    return NULL;
}

void
gnuitar_track_destroy(struct GnuitarTrack *driver)
{
    if (driver == NULL)
        return;
    if (driver->destroy_callback != NULL)
        driver->destroy_callback(driver->data);
    if (driver->chain)
        gnuitar_chain_decref(driver->chain);
}

gnuitar_error_t
gnuitar_track_add_effect(struct GnuitarTrack *driver, struct GnuitarEffect *effect)
{
    gnuitar_error_t error;

    gnuitar_mutex_lock(&driver->chain_mutex);

    if (driver->chain == NULL)
        driver->chain = gnuitar_chain_create();
    if (driver->chain == NULL)
        return GNUITAR_ERROR_MALLOC;

    error = gnuitar_chain_add_effect(driver->chain, effect);
    if (error)
        return error;

    gnuitar_mutex_unlock(&driver->chain_mutex);

    return GNUITAR_ERROR_NONE;
}

gnuitar_error_t
gnuitar_track_erase_effect(struct GnuitarTrack *driver, unsigned int index)
{
    if (driver->chain == NULL)
        return GNUITAR_ERROR_ENOENT;

    return gnuitar_chain_erase_effect(driver->chain, index);
}

int
gnuitar_track_start(struct GnuitarTrack *driver)
{
    if (driver == NULL)
        return -1;
    if (driver->start_callback == NULL)
        return -2;
    if (driver->start_callback(driver) != 0)
        return -3;

    return 0;
}

int
gnuitar_track_stop(struct GnuitarTrack *driver)
{
    if (driver == NULL)
        return -1;
    if (driver->stop_callback == NULL)
        return -2;
    if (driver->stop_callback(driver) != 0)
        return -3;

    return 0;
}

gnuitar_error_t
gnuitar_track_get_map(const struct GnuitarTrack *driver, struct GnuitarMap *map)
{
    if (driver == NULL)
        return GNUITAR_ERROR_UNKNOWN;
    if (driver->get_map_callback == NULL)
        return GNUITAR_ERROR_UNKNOWN;

    gnuitar_map_init(map);

    return driver->get_map_callback(driver, map);
}

int
gnuitar_track_get_format(const struct GnuitarTrack *driver, struct GnuitarFormat *format)
{
    if (driver == NULL)
        return -1;
    if (driver->get_format_callback == NULL)
        return -2;
    if (driver->get_format_callback(driver, format) != 0)
        return -3;
    return 0;
}

int
gnuitar_track_set_format(struct GnuitarTrack *driver, const struct GnuitarFormat *format)
{
    if (driver == NULL)
        return -1;
    if (driver->set_format_callback == NULL)
        return -2;
    if (driver->set_format_callback(driver, format) != 0)
        return -3;
    return 0;
}

void
gnuitar_format_defaults(struct GnuitarFormat *format)
{
    format->input_channels = 2;
    format->input_bits = 32;
    format->output_channels = 2;
    format->output_bits = 32;
}

#ifndef _WIN32
float procbuf[MAX_BUFFER_SIZE * MAX_CHANNELS];
float procbuf2[MAX_BUFFER_SIZE * MAX_CHANNELS];
#else
float procbuf[MAX_BUFFER_SIZE / sizeof(int16_t)];
float procbuf2[MAX_BUFFER_SIZE / sizeof(int16_t)];
#endif

struct GnuitarTrack *audio_driver = NULL;

/* default settings */
struct GnuitarMutex effectlist_lock;
#ifdef _WIN32
unsigned int overrun_threshold = 4;
unsigned int nbuffers = MAX_BUFFERS;
#endif

/* from JACK -- blindingly fast */
static inline unsigned int
prng(void)
{
    static unsigned int seed = 22222;
    seed = (seed * 96314165) + 907633515;
    return seed;
}

/* This is triangular correlated noise with frequency spectrum that increases
 * 6 dB per octave, thus most noise occurs at high frequencies. The probability
 * distribution still looks like triangle. Idea and implementation borrowed from
 * JACK. */
void
triangular_dither(struct GnuitarPacket *db, int16_t *target)
{
    static int32_t correlated_noise[MAX_CHANNELS] = { 0, 0, 0, 0 };
    uint_fast16_t i, current_channel = 0;
    
    for (i = 0; i < db->len; i += 1) {
        int32_t tmp = db->data[i];
        int32_t noise = (prng() & 0x1ff) - 256; /* -256 to 255 */
        
        tmp += noise - correlated_noise[current_channel];
        correlated_noise[current_channel] = noise;
        tmp >>= 8;
        if (tmp > 32767)
            tmp = 32767;
        if (tmp < -32768)
            tmp = -32768;
        target[i] = tmp;
        current_channel = (current_channel + 1) % db->channels;
    }
}

