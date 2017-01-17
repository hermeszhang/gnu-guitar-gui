#ifndef GNUITAR_AUDIO_DRIVER_H
#define GNUITAR_AUDIO_DRIVER_H

#include <stdint.h>

/* get rid of __restrict__ type qualifier for MS Visual C */
#ifdef _MSC_VER
#define __restrict__
#endif

#define MAX_SAMPLE (32767 << 8)

typedef float gnuitar_sample_t;

typedef struct gnuitar_packet {
    gnuitar_sample_t * __restrict__ data;
    gnuitar_sample_t * __restrict__ data_swap;
    int_fast32_t    len;
    int_fast8_t     channels;
} gnuitar_packet_t;

struct audio_driver_channels {
    unsigned int in, out;
};

typedef struct gnuitar_audio_driver {
    /** The name of the driver */
    char * name;
    /** driver specific data */
    void * data;
    /** Creates the driver data */
    void * (*create_callback)(void);
    /** Destroys the driver data */
    void (*destroy_callback)(void * data_);
    /** Sets a parameter for the driver */
    int (*set_callback)(void * data_, const char * name, const char * value);
    /** Gets a parameter for the driver */
    int (*get_callback)(const void * data_, const char * name, char ** value);
    /** Starts the audio stream */
    int (*start_callback)(void * data_);
    /** Stops the audio stream */
    int (*stop_callback)(void * data_);
/* old params */
    const char *str;
    int enabled;
    const struct audio_driver_channels *channels;
    int (*init)(void);
    void (*finish)(void);
} gnuitar_audio_driver_t;

void gnuitar_audio_driver_destroy(gnuitar_audio_driver_t * driver);

int gnuitar_audio_driver_get_param(const gnuitar_audio_driver_t * driver, const char * name, char ** value);

int gnuitar_audio_driver_set_param(gnuitar_audio_driver_t * driver, const char * name, const char * value);

int gnuitar_audio_driver_start(gnuitar_audio_driver_t * driver);

int gnuitar_audio_driver_stop(gnuitar_audio_driver_t * driver);

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

extern char alsadevice_str[64];
extern unsigned short n_input_channels;
extern unsigned short n_output_channels;
extern unsigned int sample_rate;
extern unsigned int buffer_size;

#ifndef _WIN32
extern unsigned int fragments;
#else
extern unsigned int nbuffers;
extern unsigned int overrun_threshold;
#endif

#ifndef _WIN32
extern gnuitar_sample_t procbuf[MAX_BUFFER_SIZE * MAX_CHANNELS];
extern gnuitar_sample_t procbuf2[MAX_BUFFER_SIZE * MAX_CHANNELS];
#else
/* sadly, Windows and Linux have a different idea what the size of the buffer is.
 * Linux world talks about size in frames because that is most convenient for ALSA
 * and JACK (but less so for OSS). */
extern gnuitar_sample_t procbuf[MAX_BUFFER_SIZE / sizeof(int16_t)];
extern gnuitar_sample_t procbuf2[MAX_BUFFER_SIZE / sizeof(int16_t)];
#endif

void guess_audio_driver(void);
void set_audio_driver_from_str(const char const *str);
void triangular_dither(gnuitar_packet_t *db, int16_t *target);

#endif /* GNUITAR_AUDIO_DRIVER_H */

