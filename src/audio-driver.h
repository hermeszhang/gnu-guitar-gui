#ifndef GNUITAR_AUDIO_DRIVER_H
#define GNUITAR_AUDIO_DRIVER_H

#include <stdint.h>

#include "error.h"
#include "packet.h"
#include "pump.h"
#include "map.h"

typedef struct gnuitar_format {
    unsigned int input_bits;
    unsigned int input_channels;
    unsigned int output_bits;
    unsigned int output_channels;
    unsigned int rate;
} gnuitar_format_t;

void gnuitar_format_defaults(gnuitar_format_t * format);

typedef struct gnuitar_chmap {
    unsigned int in, out;
} gnuitar_chmap_t;

typedef struct gnuitar_audio_driver gnuitar_audio_driver_t;

struct gnuitar_audio_driver {
    /** The name of the driver */
    char *name;
    /** driver specific data */
    void *data;
    /** Destroys the driver data */
    void (*destroy_callback)(void *data_);
    /** Sets a parameter for the driver */
    int (*set_format_callback)(gnuitar_audio_driver_t *driver, const gnuitar_format_t *format);
    /** Gets a parameter for the driver */
    int (*get_format_callback)(const gnuitar_audio_driver_t *driver, gnuitar_format_t *format);
    /** Gets the parameter map for the audio driver */
    gnuitar_error_t (*get_map_callback)(const gnuitar_audio_driver_t *driver, struct GnuitarMap *map);
    /** Starts the audio stream */
    int (*start_callback)(gnuitar_audio_driver_t *driver);
    /** Stops the audio stream */
    int (*stop_callback)(gnuitar_audio_driver_t *driver);
    /** The effects pump for the driver */
    gnuitar_pump_t *pump;
    /** The mutex for the pump */
    gnuitar_mutex_t pump_mutex;
    /* old params */
    int enabled;
    /** The channel maps available */
    const gnuitar_chmap_t * chmaps;
};

void gnuitar_audio_driver_destroy(gnuitar_audio_driver_t *driver);

gnuitar_error_t gnuitar_audio_driver_add_effect(gnuitar_audio_driver_t *driver, gnuitar_effect_t *effect);

gnuitar_error_t gnuitar_audio_driver_erase_effect(gnuitar_audio_driver_t *driver, unsigned int index);

gnuitar_error_t gnuitar_audio_driver_get_map(const gnuitar_audio_driver_t *driver, struct GnuitarMap *map);

int gnuitar_audio_driver_get_format(const gnuitar_audio_driver_t *driver, gnuitar_format_t *format);

int gnuitar_audio_driver_set_format(gnuitar_audio_driver_t *driver, const gnuitar_format_t *format);

int gnuitar_audio_driver_start(gnuitar_audio_driver_t *driver);

int gnuitar_audio_driver_stop(gnuitar_audio_driver_t *driver);

/* for compatibility */

typedef gnuitar_audio_driver_t audio_driver_t;

extern gnuitar_audio_driver_t *audio_driver;

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

void guess_audio_driver(void);
void set_audio_driver_from_str(const char *str);
void triangular_dither(gnuitar_packet_t *db, int16_t *target);

#endif /* GNUITAR_AUDIO_DRIVER_H */

