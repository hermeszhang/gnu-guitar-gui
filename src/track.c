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

#include "track.h"

#ifdef GNUITAR_WITH_ALSA
#include "track-alsa.h"
#endif /* GNUITAR_WITH_ALSA */

/** Initializes a track
 * @param track An unitialized track.
 * @ingroup libgnuitar-track
 */

void
gnuitar_track_init(struct GnuitarTrack *track)
{
    gnuitar_chain_init(&track->chain);
    gnuitar_driver_init(&track->driver);
}

/** Releases memory allocated by the track.
 * @param track An initialized track
 * @ingroup libgnuitar-track
 */

void
gnuitar_track_done(struct GnuitarTrack *track)
{
    gnuitar_driver_done(&track->driver);
    gnuitar_chain_done(&track->chain);
}

int
gnuitar_track_add_effect(struct GnuitarTrack *track, struct GnuitarEffect *effect)
{
    int err;

    err = gnuitar_chain_lock(&track->chain);
    if (err != 0)
        return err;

    err = gnuitar_chain_add_effect(&track->chain, effect);
    if (err != 0)
        return err;

    err = gnuitar_chain_unlock(&track->chain);
    if (err != 0)
        return err;

    return 0;
}

int
gnuitar_track_erase_effect(struct GnuitarTrack *track, unsigned int index)
{

    int err;

    err = gnuitar_chain_lock(&track->chain);
    if (err != 0)
        return err;

    err = gnuitar_chain_erase_effect(&track->chain, index);
    if (err != 0)
        return err;

    err = gnuitar_chain_unlock(&track->chain);
    if (err != 0)
        return err;

    return 0;
}

void
gnuitar_track_set_driver(struct GnuitarTrack *track, struct GnuitarDriver *driver)
{
    track->driver = *driver;
}

int
gnuitar_track_start(struct GnuitarTrack *track)
{
    return gnuitar_driver_start(&track->driver, &track->chain);
}

int
gnuitar_track_stop(struct GnuitarTrack *track)
{
    return gnuitar_driver_stop(&track->driver);
}

