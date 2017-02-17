#ifdef HAVE_ALSA

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <assert.h>
#include <pthread.h>

#include "track.h"
#include "track-alsa.h"

#include "audio-midi.h"
#include "gui.h"
#include "pump.h"

const gnuitar_chmap_t chmaps_avail[] = {
    { 1, 1 },
    { 1, 2 },
    { 2, 1 },
    { 2, 2 },
    { 0, 0 }
};

#ifdef _MSC_VER
#define gnuitar_strdup _strdup
#else
#define gnuitar_strdup strdup
#endif

static void * create_callback(void);

static void destroy_callback(void *data);

static int start_callback(gnuitar_audio_driver_t *driver);

static int stop_callback(gnuitar_audio_driver_t *driver);

static gnuitar_error_t get_map_callback(const gnuitar_audio_driver_t *driver, struct GnuitarMap *map);

static int get_format_callback(const gnuitar_audio_driver_t *driver, gnuitar_format_t *format);

static int set_format_callback(gnuitar_audio_driver_t *driver, const gnuitar_format_t *format);

static void * alsa_audio_thread(void *data);

static int alsa_configure_audio(snd_pcm_t *device,
                                unsigned int *periods,
                                unsigned int *period_size,
                                int channels,
                                unsigned long int *rate,
                                unsigned int *bits,
                                int adapting);

gnuitar_audio_driver_t *
gnuitar_alsa_driver_create(void)
{
    gnuitar_audio_driver_t *driver;

    driver = malloc(sizeof(*driver));
    if (driver == NULL)
        return NULL;

    driver->name = gnuitar_strdup("ALSA");
    if (driver->name == NULL) {
        free(driver);
        return NULL;
    }

    gnuitar_mutex_init(&driver->pump_mutex);
    driver->pump = gnuitar_pump_create();

    driver->destroy_callback = destroy_callback;
    driver->start_callback = start_callback;
    driver->stop_callback = stop_callback;
    driver->set_format_callback = set_format_callback;
    driver->get_map_callback = get_map_callback;
    driver->get_format_callback = get_format_callback;

    driver->data = create_callback();
    if (driver->data == NULL) {
        free(driver->name);
        free(driver);
        return NULL;
    }

    /* old params */
    driver->enabled = 1;
    driver->chmaps = chmaps_avail;

    return driver;
}

int
gnuitar_alsa_available(void)
{
    return 1;
}

static void *
create_callback(void)
{
    gnuitar_alsa_driver_t *alsa_driver;

    alsa_driver = malloc(sizeof(*alsa_driver));
    if (alsa_driver == NULL)
        return NULL;

    alsa_driver->input_name = "plughw:1,0";
    alsa_driver->input_pcm = NULL;
    alsa_driver->input_channels = 2;
    alsa_driver->input_bits = 32;

    alsa_driver->output_name = "default";
    alsa_driver->output_pcm = NULL;
    alsa_driver->output_channels = 2;
    alsa_driver->output_bits = 32;

    alsa_driver->period_size = 128;
    alsa_driver->periods = 4;
    alsa_driver->rate = 48000;

    alsa_driver->keep_thread_running = 0;

    return alsa_driver;
}

static void
destroy_callback(void *data)
{
    gnuitar_alsa_driver_t *alsa_driver;
    alsa_driver = (gnuitar_alsa_driver_t *)(data);
    if (alsa_driver != NULL) {
        if (alsa_driver->input_pcm != NULL)
            snd_pcm_close(alsa_driver->input_pcm);
        if (alsa_driver->input_name != NULL)
            free(alsa_driver->input_name);
        if (alsa_driver->output_pcm != NULL)
            snd_pcm_close(alsa_driver->output_pcm);
        if (alsa_driver->output_name != NULL)
            free(alsa_driver->output_name);
    }
}

static int
start_callback(gnuitar_audio_driver_t *driver)
{
    gnuitar_alsa_driver_t *alsa_driver;

    alsa_driver = (gnuitar_alsa_driver_t *)(driver->data);
    if (alsa_driver == NULL)
        return -1;

    if (snd_pcm_open(&alsa_driver->input_pcm,
                     alsa_driver->input_name,
                     SND_PCM_STREAM_CAPTURE,
                     0 /* mode flags */) != 0) {
        return -2;
    }

    if (alsa_configure_audio(alsa_driver->input_pcm,
                             &alsa_driver->periods,
                             &alsa_driver->period_size,
                             alsa_driver->input_channels,
                             &alsa_driver->rate,
                             &alsa_driver->input_bits,
                             1 /* 1 means values may be changed */) != 0) {
        return -3;
    }

    if (snd_pcm_open(&alsa_driver->output_pcm,
                     alsa_driver->output_name,
                     SND_PCM_STREAM_PLAYBACK,
                     0 /* mode flags */) != 0) {
        return -2;
    }

    if (alsa_configure_audio(alsa_driver->output_pcm,
                             &alsa_driver->periods,
                             &alsa_driver->period_size,
                             alsa_driver->output_channels,
                             &alsa_driver->rate,
                             &alsa_driver->output_bits,
                             1 /* 1 means values may be changed */) != 0) {
        return -3;
    }

    alsa_driver->keep_thread_running = 1;

    if (pthread_create(&alsa_driver->thread, NULL, alsa_audio_thread, driver)) {
        return -4;
    }

    return 0;
}

static int
stop_callback(gnuitar_audio_driver_t *driver)
{
    gnuitar_alsa_driver_t *alsa_driver;

    alsa_driver = (gnuitar_alsa_driver_t *)(driver->data);
    if (alsa_driver == NULL)
        return -1;

    alsa_driver->keep_thread_running = 0;

    if (pthread_join(alsa_driver->thread, NULL) != 0)
        return -1;

    return 0;
}

static gnuitar_error_t
get_map_callback(const gnuitar_audio_driver_t *driver, struct GnuitarMap *map)
{
    gnuitar_error_t error;

    (void) driver;

    error = gnuitar_map_define(map, "Rate", GNUITAR_MAP_TYPE_UINT32);
    if (error)
        return error;

    error = gnuitar_map_define(map, "Periods", GNUITAR_MAP_TYPE_UINT32);
    if (error)
        return error;

    error = gnuitar_map_define(map, "Period Size", GNUITAR_MAP_TYPE_UINT32);
    if (error)
        return error;

    error = gnuitar_map_define(map, "Input Device", GNUITAR_MAP_TYPE_STRING);
    if (error)
        return error;

    error = gnuitar_map_define(map, "Output Device", GNUITAR_MAP_TYPE_STRING);
    if (error)
        return error;

    return GNUITAR_ERROR_NONE;
}

static int
get_format_callback(const gnuitar_audio_driver_t *driver, gnuitar_format_t *format)
{
    const gnuitar_alsa_driver_t *alsa_driver;

    alsa_driver = (gnuitar_alsa_driver_t *)(driver->data);
    if (alsa_driver == NULL)
        return -1;

    format->output_bits = alsa_driver->output_bits;
    format->output_channels = alsa_driver->output_channels;

    format->input_bits = alsa_driver->input_bits;
    format->input_channels = alsa_driver->input_channels;

    format->rate = alsa_driver->rate;

    return 0;
}

static int
set_format_callback(gnuitar_audio_driver_t *driver, const gnuitar_format_t *format)
{
    gnuitar_alsa_driver_t *alsa_driver;

    alsa_driver = (gnuitar_alsa_driver_t *)(driver->data);
    if (alsa_driver == NULL)
        return -1;

    /* not implemented */
    (void) format;

    return -2;
}

static void *
alsa_audio_thread(void *data)
{
    unsigned int i;
    int inframes, outframes;
    gnuitar_packet_t db;
    unsigned int restarting = 0;
    unsigned int data_size = 0;

    gnuitar_audio_driver_t *driver;
    gnuitar_alsa_driver_t *alsa_driver;

    snd_pcm_t * input_pcm;
    snd_pcm_t * output_pcm;

    driver = (gnuitar_audio_driver_t *)(data);
    if (driver == NULL)
        return NULL;

    alsa_driver = (gnuitar_alsa_driver_t *)(driver->data);
    if (alsa_driver == NULL)
        return NULL;

    input_pcm = alsa_driver->input_pcm;
    if (input_pcm == NULL)
        return NULL;

    output_pcm = alsa_driver->output_pcm;
    if (output_pcm == NULL)
        return NULL;

    data_size = (alsa_driver->output_channels
               * alsa_driver->period_size
               * alsa_driver->output_bits) / 8;

    db.data = malloc(data_size);
    if (db.data == NULL)
        return NULL;

    db.data_swap = malloc(data_size);
    if (db.data_swap == NULL) {
        free(db.data);
        return NULL;
    }

    db.len = alsa_driver->output_channels
           * alsa_driver->period_size;
    db.channels = alsa_driver->output_channels;
    db.rate = alsa_driver->rate;

    /* frame counts are always the same to both read and write */
    while (alsa_driver->keep_thread_running) {

        if (restarting) {
            restarting = 0;
            /* drop any output we might got and stop */
            snd_pcm_drop(input_pcm);
            snd_pcm_drop(output_pcm);
            /* prepare for use */
            snd_pcm_prepare(input_pcm);
            snd_pcm_prepare(output_pcm);
            /* prefill audio area */
            memset(db.data, 0, data_size);
            for (i = 0; i < alsa_driver->periods; i += 1)
                if (snd_pcm_avail_update(output_pcm) > 0)
                    snd_pcm_writei(output_pcm, db.data, db.len / db.channels);
        }

        while ((inframes = snd_pcm_readi(input_pcm, db.data, db.len / db.channels)) < 0) {
            //gnuitar_printf( "Input buffer overrun\n");
            restarting = 1;
            snd_pcm_prepare(input_pcm);
        }
        db.len = inframes * db.channels;

        gnuitar_packet_mul(&db, MAX_SAMPLE);

        gnuitar_mutex_lock(&driver->pump_mutex);
        gnuitar_pump_process(driver->pump, &db);
        gnuitar_mutex_unlock(&driver->pump_mutex);

        gnuitar_packet_div(&db, MAX_SAMPLE);
        /* write output */
        while ((outframes = snd_pcm_writei(output_pcm, db.data, db.len / db.channels)) < 0) {
            restarting = 1;
            snd_pcm_prepare(output_pcm);
        }
    }
    return NULL;
}

/* The adapting flag allows the first invocation to change sampling parameters.
 * On the second call, adapting is disabled. This is done to force identical
 * parameters on the two devices, which may not even be same physical
 * hardware. The return code from this function is an error code. */
static int
alsa_configure_audio(snd_pcm_t *device, unsigned int *fragments, unsigned int *frames, int channels, unsigned long int *rate, unsigned int *bits, int adapting)
{
    snd_pcm_hw_params_t *hw_params;
    int                 err;
    unsigned int        tmp;
    snd_pcm_uframes_t   alsa_frames;
    
    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        gnuitar_printf( "can't allocate parameter structure: %s\n",
                 snd_strerror(err));
	return 1;
    }
    
    if ((err = snd_pcm_hw_params_any(device, hw_params)) < 0) {
        gnuitar_printf( "can't initialize parameter structure: %s\n",
                 snd_strerror(err));
        snd_pcm_hw_params_free(hw_params);
	return 1;
    }

    if ((err = snd_pcm_hw_params_set_access(device, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        gnuitar_printf("can't set access type RW_INTERLEAVE (try using plughw): %s\n",
                 snd_strerror(err));
        snd_pcm_hw_params_free(hw_params);
	return 1;
    }

    /* XXX we should support floating-point sample format */
    if ((err = snd_pcm_hw_params_set_format(device, hw_params, SND_PCM_FORMAT_FLOAT)) < 0) {
        if ((err = snd_pcm_hw_params_set_format(device, hw_params, SND_PCM_FORMAT_S16)) < 0) {
            gnuitar_printf("can't set sample format to neither 32 nor 16 bits: %s\n",
                            snd_strerror(err));
            snd_pcm_hw_params_free(hw_params);
	    return 1;
        } else {
            *bits = 16;
        }
    } else {
        *bits = 32;
    }

    if ((err = snd_pcm_hw_params_set_channels(device, hw_params, channels)) < 0) {
        gnuitar_printf( "can't set channel count %d: %s\n", channels, snd_strerror(err));
        snd_pcm_hw_params_free(hw_params);
        return 1;
    }

    /* ALSA has poor quality resampling. User is much better off without
     * resampling and GNUitar adapted to proper frequency than with it. */
#if(SND_LIB_MAJOR >=1 && SND_LIB_MINOR >= 0 && SND_LIB_SUBMINOR >8)
    if ((err = snd_pcm_hw_params_set_rate_resample(device, hw_params, 0)) < 0) {
        gnuitar_printf("warning: can't disallow ALSA-lib resampling: %s\n", snd_strerror(err));
        /* this isn't fatal, though. We'll just continue. */
    }
#endif

    if (adapting) {
        /* Adapting path: choose values for the tunables:
         * sampling rate, fragment size, number of fragments. */
        tmp = *rate;
        if ((err = snd_pcm_hw_params_set_rate_near(device, hw_params, &tmp, 0)) < 0) {
            gnuitar_printf("can't set sample rate %d: %s\n", tmp, snd_strerror(err));
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        if (tmp != *rate) {
            gnuitar_printf("info: hardware adjusted sample rate to %d.\n", tmp);
            *rate = tmp;
        }

        /* tell alsa how many periods we would like to have */
        if ((err = snd_pcm_hw_params_set_periods(device, hw_params, *fragments, 0)) < 0) {
            gnuitar_printf("warning: can't set fragments %d value on ALSA device: %s\n", *fragments, snd_strerror(err));
            /* If it doesn't work, well, no matter. Next time *fragments will have
             * another value which hopefully works. And set_buffer_time_near() may
             * still work. The message to take home is: ALSA is difficult to program. */
        }

        /* let the sound driver pick period size as appropriate. */
        tmp = (float) (*frames * *fragments) / (*rate) * 1E6;

        if ((err = snd_pcm_hw_params_set_buffer_time_near(device, hw_params, &tmp, NULL)) < 0) {
            gnuitar_printf("can't set buffer time near %d: %s\n", tmp, snd_strerror(err));
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        /* obtain the frames and fragments chosen by ALSA */
        if ((err = snd_pcm_hw_params_get_period_size(hw_params, &alsa_frames, NULL)) < 0) {
            gnuitar_printf( "can't get period size value from ALSA (read: %d): %s\n", (int) alsa_frames, snd_strerror(err));
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        *frames = alsa_frames;
        if ((err = snd_pcm_hw_params_get_periods(hw_params, fragments, NULL)) < 0) {
            gnuitar_printf( "can't get fragments value from ALSA (read: %d): %s\n", *fragments, snd_strerror(err));
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
    } else {
        /* use the fixed values given before */
        tmp = *rate;
        if ((err = snd_pcm_hw_params_set_rate(device, hw_params, tmp, 0)) < 0) {
            gnuitar_printf("can't set sample rate %d: %s\n", tmp, snd_strerror(err));
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        if ((err = snd_pcm_hw_params_set_period_size(device, hw_params, *frames, 0)) < 0) {
            gnuitar_printf( "can't set period size to %d frames: %s\n", (int) *frames, snd_strerror(err));
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        
        if ((err = snd_pcm_hw_params_set_periods(device, hw_params, *fragments, 0)) < 0) {
            gnuitar_printf("can't set periods to %d: %s\n", *fragments, snd_strerror(err));
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }        
    }

    if ((err = snd_pcm_hw_params(device, hw_params)) < 0) {
        gnuitar_printf("Error setting HW params: %s\n", snd_strerror(err));
        snd_pcm_hw_params_free(hw_params);
        return 1;
    }
    snd_pcm_hw_params_free(hw_params);
    return 0;
}

#else /* HAVE_ALSA */

int
gnuitar_alsa_available(void)
{
    return 0;
}

#endif /* HAVE_ALSA */

