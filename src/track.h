#ifndef GNUITAR_TRACK_H
#define GNUITAR_TRACK_H

#include "chain.h"
#include "driver.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup libgnuitar-track Guitar Tracks
 * Guitar Track API
 */

/** A guitar track.
 * @ingroup libgnuitar-track
 */

struct GnuitarTrack {
    /** The audio driver for the track */
    struct GnuitarDriver driver;
    /** The effects chain for the track */
    struct GnuitarChain chain;
};

void gnuitar_track_init(struct GnuitarTrack *track);

void gnuitar_track_done(struct GnuitarTrack *track);

int gnuitar_track_add_effect(struct GnuitarTrack *track, struct GnuitarEffect *effect);

int gnuitar_track_erase_effect(struct GnuitarTrack *track, unsigned int index);

int gnuitar_track_move_effect(struct GnuitarTrack *track, size_t dst, size_t src);

void gnuitar_track_set_driver(struct GnuitarTrack *track, struct GnuitarDriver *driver);

int gnuitar_track_start(struct GnuitarTrack *track);

int gnuitar_track_stop(struct GnuitarTrack *track);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_TRACK_H */

