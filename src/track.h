#ifndef GNUITAR_TRACK_H
#define GNUITAR_TRACK_H

#include <stdint.h>

#include "error.h"
#include "packet.h"
#include "chain.h"
#include "map.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct GnuitarFormat {
    unsigned int input_bits;
    unsigned int input_channels;
    unsigned int output_bits;
    unsigned int output_channels;
    unsigned int rate;
};

void gnuitar_format_defaults(struct GnuitarFormat * format);

struct GnuitarChannelMap {
    /** The number of input channels */
    unsigned int in;
    /** The number of output channels */
    unsigned int out;
};

struct GnuitarTrack {
    /** The name of the track */
    char *name;
    /** track specific data */
    void *data;
    /** Destroys the track data */
    void (*destroy_callback)(void *data_);
    /** Sets a parameter for the track */
    int (*set_format_callback)(struct GnuitarTrack *track, const struct GnuitarFormat *format);
    /** Gets a parameter for the track */
    int (*get_format_callback)(const struct GnuitarTrack *track, struct GnuitarFormat *format);
    /** Gets the parameter map for the audio track */
    gnuitar_error_t (*get_map_callback)(const struct GnuitarTrack *track, struct GnuitarMap *map);
    /** Starts the audio stream */
    int (*start_callback)(struct GnuitarTrack *track);
    /** Stops the audio stream */
    int (*stop_callback)(struct GnuitarTrack *track);
    /** The effects pump for the track */
    struct GnuitarChain *chain;
    /** The mutex for the pump */
    struct GnuitarMutex chain_mutex;
    /* old params */
    int enabled;
    /** The channel maps available */
    const struct GnuitarChannelMap* chmaps;
};

struct GnuitarTrack * gnuitar_track_create(const char * name);

void gnuitar_track_destroy(struct GnuitarTrack *track);

gnuitar_error_t gnuitar_track_add_effect(struct GnuitarTrack *track, struct GnuitarEffect *effect);

gnuitar_error_t gnuitar_track_erase_effect(struct GnuitarTrack *track, unsigned int index);

gnuitar_error_t gnuitar_track_get_map(const struct GnuitarTrack *track, struct GnuitarMap *map);

int gnuitar_track_get_format(const struct GnuitarTrack *track, struct GnuitarFormat *format);

int gnuitar_track_set_format(struct GnuitarTrack *track, const struct GnuitarFormat *format);

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

