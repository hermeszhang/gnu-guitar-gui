#ifndef GNUITAR_TRACK_H
#define GNUITAR_TRACK_H

#include "packet.h"
#include "chain.h"
#include "map.h"
#include "utils.h"

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

/* for compatibility */

#ifdef _WIN32
#define MAX_BUFFERS 1024 /* number of input/output sound buffers */
#endif

#define MIN_BUFFER_SIZE 128
#define MAX_BUFFER_SIZE 65536
#define MAX_CHANNELS 4
#define MAX_SAMPLE_RATE 48000

#ifdef _WIN32
extern unsigned int nbuffers;
extern unsigned int overrun_threshold;
#endif

#ifdef _WIN32
/* sadly, Windows and Linux have a different idea what the size of the buffer is.
 * Linux world talks about size in frames because that is most convenient for ALSA
 * and JACK (but less so for OSS). */
extern gnuitar_sample_t procbuf[MAX_BUFFER_SIZE / sizeof(int16_t)];
extern gnuitar_sample_t procbuf2[MAX_BUFFER_SIZE / sizeof(int16_t)];
#endif

void guess_track(void);
void set_track_from_str(const char *str);
void triangular_dither(struct GnuitarPacket *db, int16_t *target);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_TRACK_H */

