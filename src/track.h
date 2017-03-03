#ifndef GNUITAR_TRACK_H
#define GNUITAR_TRACK_H

#include "packet.h"
#include "chain.h"
#include "map.h"
#include "mutex.h"

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
    /** track specific data */
    void *data;
    /** Destroys the track data */
    void (*done)(void *data_);
    /** Gets the parameter map for the audio track */
    int (*get_map)(const struct GnuitarTrack *track, struct GnuitarMap *map);
    /** Sets the parameter map of the audio track */
    int (*set_map)(struct GnuitarTrack *track, const struct GnuitarMap *map);
    /** Starts the audio stream */
    int (*start)(struct GnuitarTrack *track);
    /** Stops the audio stream */
    int (*stop)(struct GnuitarTrack *track);
    /** The effects pump for the track */
    struct GnuitarChain chain;
    /** The mutex for the pump */
    struct GnuitarMutex chain_mutex;
};

int gnuitar_track_init(struct GnuitarTrack *track, const char *name);

void gnuitar_track_done(struct GnuitarTrack *track);

int gnuitar_track_add_effect(struct GnuitarTrack *track, struct GnuitarEffect *effect);

int gnuitar_track_erase_effect(struct GnuitarTrack *track, unsigned int index);

int gnuitar_track_move_effect(struct GnuitarTrack *track, size_t dst, size_t src);

int gnuitar_track_get_map(const struct GnuitarTrack *track, struct GnuitarMap *map);

int gnuitar_track_set_map(struct GnuitarTrack *track, const struct GnuitarMap *map);

int gnuitar_track_start(struct GnuitarTrack *track);

int gnuitar_track_stop(struct GnuitarTrack *track);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_TRACK_H */

