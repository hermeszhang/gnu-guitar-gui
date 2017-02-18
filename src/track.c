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
        return gnuitar_alsa_driver_create();
    }
#endif /* GNUITAR_WITH_ALSA */
    if (strcmp(name, "Null") == 0) {
        /* TODO */
        return NULL;
    }
    return NULL;
}

void
gnuitar_audio_driver_destroy(struct GnuitarTrack *driver)
{
    if (driver == NULL)
        return;
    if (driver->destroy_callback != NULL)
        driver->destroy_callback(driver->data);
    if (driver->pump)
        gnuitar_pump_decref(driver->pump);
}

gnuitar_error_t
gnuitar_audio_driver_add_effect(struct GnuitarTrack *driver, struct GnuitarEffect *effect)
{
    gnuitar_error_t error;

    gnuitar_mutex_lock(&driver->pump_mutex);

    if (driver->pump == NULL)
        driver->pump = gnuitar_pump_create();
    if (driver->pump == NULL)
        return GNUITAR_ERROR_MALLOC;

    error = gnuitar_pump_add_effect(driver->pump, effect);
    if (error)
        return error;

    gnuitar_mutex_unlock(&driver->pump_mutex);

    return GNUITAR_ERROR_NONE;
}

gnuitar_error_t
gnuitar_audio_driver_erase_effect(struct GnuitarTrack *driver, unsigned int index)
{
    if (driver->pump == NULL)
        return GNUITAR_ERROR_ENOENT;

    return gnuitar_pump_erase_effect(driver->pump, index);
}

int
gnuitar_audio_driver_start(struct GnuitarTrack *driver)
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
gnuitar_audio_driver_stop(struct GnuitarTrack *driver)
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
gnuitar_audio_driver_get_map(const struct GnuitarTrack *driver, struct GnuitarMap *map)
{
    if (driver == NULL)
        return GNUITAR_ERROR_UNKNOWN;
    if (driver->get_map_callback == NULL)
        return GNUITAR_ERROR_UNKNOWN;

    gnuitar_map_init(map);

    return driver->get_map_callback(driver, map);
}

int
gnuitar_audio_driver_get_format(const struct GnuitarTrack *driver, struct GnuitarFormat *format)
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
gnuitar_audio_driver_set_format(struct GnuitarTrack *driver, const struct GnuitarFormat *format)
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

void
guess_audio_driver(void)
{
    audio_driver = NULL;

#ifdef HAVE_JACK
    if (jack_available()) {
        audio_driver = &jack_driver;
    } else
#endif
#ifdef HAVE_ALSA
    if (gnuitar_alsa_available()) {
        audio_driver = gnuitar_alsa_driver_create();
    } else
#endif
#ifdef HAVE_OSS
    if (oss_available()) {
        audio_driver = &oss_driver;
    } else
#endif
#ifdef HAVE_DSOUND
    if (audio_driver == NULL) {
	audio_driver = &dsound_driver;
    } else
#endif
#ifdef HAVE_WINMM
    if (audio_driver == NULL) {
	audio_driver = &winmm_driver;
    } else
#endif
        return;
}

void
set_audio_driver_from_str(const char const *tmp)
{
#ifdef HAVE_JACK
    if (strcmp(tmp, "JACK") == 0) {
        audio_driver = &jack_driver;
    } else
#endif
#ifdef GNUITAR_WITH_ALSA
    if (strcmp(tmp, "ALSA") == 0) {
        audio_driver = gnuitar_alsa_driver_create();
    } else
#endif /* GNUITAR_WITH_ALSA */
#ifdef HAVE_OSS
    if (strcmp(tmp, "OSS") == 0) {
        audio_driver = &oss_driver;
    } else
#endif
#ifdef HAVE_DSOUND
    if(strcmp(tmp, "DirectX") == 0) {
        audio_driver = &dsound_driver;
    } else
#endif
#ifdef HAVE_WINMM
    if(strcmp(tmp, "MMSystem") == 0) {
        audio_driver = &winmm_driver;
        buffer_size = pow(2, (int) (log(buffer_size) / log(2)));
    } else
#endif
        return;
}

