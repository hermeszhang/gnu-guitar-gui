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
#include "audio-driver.h"
#include "audio-alsa.h"
#include "audio-oss.h"
#include "audio-jack.h"
#include "audio-winmm.h"
#include "audio-dsound.h"

void
gnuitar_packet_mul(gnuitar_packet_t *packet, float n)
{
    unsigned int i;
    for (i = 0; i < packet->len; i++) {
        packet->data[i] *= n;
    }
}

void
gnuitar_packet_div(gnuitar_packet_t *packet, float n)
{
    unsigned int i;
    for (i = 0; i < packet->len; i++) {
        packet->data[i] /= n;
    }
}

void
gnuitar_audio_driver_destroy(gnuitar_audio_driver_t *driver)
{
    if ((driver != NULL) && (driver->destroy_callback != NULL))
        driver->destroy_callback(driver->data);
}

int
gnuitar_audio_driver_start(gnuitar_audio_driver_t *driver)
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
gnuitar_audio_driver_stop(gnuitar_audio_driver_t *driver)
{
    if (driver == NULL)
        return -1;
    if (driver->stop_callback == NULL)
        return -2;
    if (driver->stop_callback(driver) != 0)
        return -3;

    return 0;
}

int
gnuitar_audio_driver_get_format(const gnuitar_audio_driver_t *driver, gnuitar_format_t *format)
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
gnuitar_audio_driver_set_format(gnuitar_audio_driver_t *driver, const gnuitar_format_t *format)
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
gnuitar_format_defaults(gnuitar_format_t *format)
{
    format->input_channels = 2;
    format->input_bits = 32;
    format->output_channels = 2;
    format->output_bits = 32;
}

#ifndef _WIN32
gnuitar_sample_t procbuf[MAX_BUFFER_SIZE * MAX_CHANNELS];
gnuitar_sample_t procbuf2[MAX_BUFFER_SIZE * MAX_CHANNELS];
#else
gnuitar_sample_t procbuf[MAX_BUFFER_SIZE / sizeof(int16_t)];
gnuitar_sample_t procbuf2[MAX_BUFFER_SIZE / sizeof(int16_t)];
#endif

audio_driver_t  *audio_driver = NULL;

/* default settings */
gnuitar_mutex_t effectlist_lock;
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
triangular_dither(gnuitar_packet_t *db, int16_t *target)
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
#ifdef HAVE_ALSA
    if (strcmp(tmp, "ALSA") == 0) {
        audio_driver = gnuitar_alsa_driver_create();
    } else
#endif
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
