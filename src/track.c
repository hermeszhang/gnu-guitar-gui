/*
 * Audio track interface to reduce file interdependencies and give home to
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
 * - construct audio-track.c to hold globals and provide some utility
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

/** Initializes a track, using a specified API.
 * @param track An unitialized track.
 * @param api_name The name of the API to use.
 * @returns On success, zero.
 *  If @p api_name is an invalid name, EINVAL is returned.
 *  If @p api_name is NULL, EFAULT is returned.
 * @ingroup libgnuitar-track
 */

int
gnuitar_track_init(struct GnuitarTrack *track, const char *api_name)
{
    gnuitar_mutex_init(&track->chain_mutex);
    gnuitar_chain_init(&track->chain);

    if (api_name == NULL)
        return EFAULT;

#ifdef GNUITAR_WITH_ALSA
    if (strcmp(api_name, "ALSA") == 0) {
        return gnuitar_alsa_track_init(track);
    }
#endif /* GNUITAR_WITH_ALSA */
    if (strcmp(api_name, "Null") == 0) {
        /* TODO */
	return EFAULT;
    }
    return EINVAL;
}

/** Releases memory allocated by the track.
 * @param track An initialized track
 * @ingroup libgnuitar-track
 */

void
gnuitar_track_done(struct GnuitarTrack *track)
{
    if (track->done != NULL)
        track->done(track->data);

    gnuitar_chain_done(&track->chain);
}

int
gnuitar_track_add_effect(struct GnuitarTrack *track, struct GnuitarEffect *effect)
{
    int err;

    err = gnuitar_mutex_lock(&track->chain_mutex);
    if (err != 0)
        return err;

    err = gnuitar_chain_add_effect(&track->chain, effect);
    if (err != 0)
        return err;

    err = gnuitar_mutex_unlock(&track->chain_mutex);
    if (err != 0)
        return err;

    return 0;
}

int
gnuitar_track_erase_effect(struct GnuitarTrack *track, unsigned int index)
{

    int err;

    err = gnuitar_mutex_lock(&track->chain_mutex);
    if (err != 0)
        return err;

    err = gnuitar_chain_erase_effect(&track->chain, index);
    if (err != 0)
        return err;

    err = gnuitar_mutex_unlock(&track->chain_mutex);
    if (err != 0)
        return err;

    return 0;
}

int
gnuitar_track_start(struct GnuitarTrack *track)
{
    if (track == NULL)
        return -1;
    if (track->start == NULL)
        return -2;
    if (track->start(track) != 0)
        return -3;

    return 0;
}

int
gnuitar_track_stop(struct GnuitarTrack *track)
{
    if (track->stop == NULL)
        return EFAULT;

    return track->stop(track);
}

int
gnuitar_track_get_map(const struct GnuitarTrack *track, struct GnuitarMap *map)
{
    if (track->get_map == NULL)
        return EFAULT;

    gnuitar_map_init(map);

    return track->get_map(track, map);
}

#ifndef _WIN32
float procbuf[MAX_BUFFER_SIZE * MAX_CHANNELS];
float procbuf2[MAX_BUFFER_SIZE * MAX_CHANNELS];
#else
float procbuf[MAX_BUFFER_SIZE / sizeof(int16_t)];
float procbuf2[MAX_BUFFER_SIZE / sizeof(int16_t)];
#endif

struct GnuitarTrack *audio_track = NULL;

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

