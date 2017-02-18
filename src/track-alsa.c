#include "track.h"
#include "track-alsa.h"

const struct GnuitarChannelMap chmaps_avail[] = {
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

static int start_callback(struct GnuitarTrack *track);

static int stop_callback(struct GnuitarTrack *track);

static gnuitar_error_t get_map_callback(const struct GnuitarTrack *track, struct GnuitarMap *map);

static int get_format_callback(const struct GnuitarTrack *track, struct GnuitarFormat *format);

static int set_format_callback(struct GnuitarTrack *track, const struct GnuitarFormat *format);

static void * alsa_audio_thread(void *data);

static int alsa_configure_audio(snd_pcm_t *device,
                                unsigned int *periods,
                                unsigned int *period_size,
                                int channels,
                                unsigned long int *rate,
                                unsigned int *bits,
                                int adapting);

struct GnuitarTrack *
gnuitar_alsa_track_create(void)
{
    struct GnuitarTrack *track;

    track = malloc(sizeof(*track));
    if (track == NULL)
        return NULL;

    track->name = gnuitar_strdup("ALSA");
    if (track->name == NULL) {
        free(track);
        return NULL;
    }

    gnuitar_mutex_init(&track->pump_mutex);
    track->pump = gnuitar_pump_create();

    track->destroy_callback = destroy_callback;
    track->start_callback = start_callback;
    track->stop_callback = stop_callback;
    track->set_format_callback = set_format_callback;
    track->get_map_callback = get_map_callback;
    track->get_format_callback = get_format_callback;

    track->data = create_callback();
    if (track->data == NULL) {
        free(track->name);
        free(track);
        return NULL;
    }

    /* old params */
    track->enabled = 1;
    track->chmaps = chmaps_avail;

    return track;
}

int
gnuitar_alsa_available(void)
{
    return 1;
}

static void *
create_callback(void)
{
    struct GnuitarAlsaTrack *alsa_track;

    alsa_track = malloc(sizeof(*alsa_track));
    if (alsa_track == NULL)
        return NULL;

    alsa_track->input_name = gnuitar_strdup("default");
    alsa_track->input_pcm = NULL;
    alsa_track->input_channels = 2;
    alsa_track->input_bits = 32;

    alsa_track->output_name = gnuitar_strdup("default");
    alsa_track->output_pcm = NULL;
    alsa_track->output_channels = 2;
    alsa_track->output_bits = 32;

    alsa_track->period_size = 128;
    alsa_track->periods = 4;
    alsa_track->rate = 48000;

    alsa_track->keep_thread_running = 0;

    return alsa_track;
}

static void
destroy_callback(void *data)
{
    struct GnuitarAlsaTrack *alsa_track;
    alsa_track = (struct GnuitarAlsaTrack *)(data);
    if (alsa_track != NULL) {
        if (alsa_track->input_pcm != NULL)
            snd_pcm_close(alsa_track->input_pcm);
        if (alsa_track->input_name != NULL)
            free(alsa_track->input_name);
        if (alsa_track->output_pcm != NULL)
            snd_pcm_close(alsa_track->output_pcm);
        if (alsa_track->output_name != NULL)
            free(alsa_track->output_name);
    }
}

static int
start_callback(struct GnuitarTrack *track)
{
    struct GnuitarAlsaTrack *alsa_track;

    alsa_track = (struct GnuitarAlsaTrack *)(track->data);
    if (alsa_track == NULL)
        return -1;

    if (snd_pcm_open(&alsa_track->input_pcm,
                     alsa_track->input_name,
                     SND_PCM_STREAM_CAPTURE,
                     0 /* mode flags */) != 0) {
        return -2;
    }

    if (alsa_configure_audio(alsa_track->input_pcm,
                             &alsa_track->periods,
                             &alsa_track->period_size,
                             alsa_track->input_channels,
                             &alsa_track->rate,
                             &alsa_track->input_bits,
                             1 /* 1 means values may be changed */) != 0) {
        return -3;
    }

    if (snd_pcm_open(&alsa_track->output_pcm,
                     alsa_track->output_name,
                     SND_PCM_STREAM_PLAYBACK,
                     0 /* mode flags */) != 0) {
        return -2;
    }

    if (alsa_configure_audio(alsa_track->output_pcm,
                             &alsa_track->periods,
                             &alsa_track->period_size,
                             alsa_track->output_channels,
                             &alsa_track->rate,
                             &alsa_track->output_bits,
                             1 /* 1 means values may be changed */) != 0) {
        return -3;
    }

    alsa_track->keep_thread_running = 1;

    if (pthread_create(&alsa_track->thread, NULL, alsa_audio_thread, track)) {
        return -4;
    }

    return 0;
}

static int
stop_callback(struct GnuitarTrack *track)
{
    struct GnuitarAlsaTrack *alsa_track;

    alsa_track = (struct GnuitarAlsaTrack *)(track->data);
    if (alsa_track == NULL)
        return -1;

    alsa_track->keep_thread_running = 0;

    if (pthread_join(alsa_track->thread, NULL) != 0)
        return -1;

    return 0;
}

static gnuitar_error_t
get_map_callback(const struct GnuitarTrack *track, struct GnuitarMap *map)
{
    gnuitar_error_t error;

    (void) track;

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
get_format_callback(const struct GnuitarTrack *track, struct GnuitarFormat *format)
{
    const struct GnuitarAlsaTrack *alsa_track;

    alsa_track = (struct GnuitarAlsaTrack *)(track->data);
    if (alsa_track == NULL)
        return -1;

    format->output_bits = alsa_track->output_bits;
    format->output_channels = alsa_track->output_channels;

    format->input_bits = alsa_track->input_bits;
    format->input_channels = alsa_track->input_channels;

    format->rate = alsa_track->rate;

    return 0;
}

static int
set_format_callback(struct GnuitarTrack *track, const struct GnuitarFormat *format)
{
    struct GnuitarAlsaTrack *alsa_track;

    alsa_track = (struct GnuitarAlsaTrack *)(track->data);
    if (alsa_track == NULL)
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
    struct GnuitarPacket db;
    unsigned int restarting = 0;
    unsigned int data_size = 0;

    struct GnuitarTrack *track;
    struct GnuitarAlsaTrack *alsa_track;

    snd_pcm_t * input_pcm;
    snd_pcm_t * output_pcm;

    track = (struct GnuitarTrack *)(data);
    if (track == NULL)
        return NULL;

    alsa_track = (struct GnuitarAlsaTrack *)(track->data);
    if (alsa_track == NULL)
        return NULL;

    input_pcm = alsa_track->input_pcm;
    if (input_pcm == NULL)
        return NULL;

    output_pcm = alsa_track->output_pcm;
    if (output_pcm == NULL)
        return NULL;

    data_size = (alsa_track->output_channels
               * alsa_track->period_size
               * alsa_track->output_bits) / 8;

    db.data = malloc(data_size);
    if (db.data == NULL)
        return NULL;

    db.data_swap = malloc(data_size);
    if (db.data_swap == NULL) {
        free(db.data);
        return NULL;
    }

    db.len = alsa_track->output_channels
           * alsa_track->period_size;
    db.channels = alsa_track->output_channels;
    db.rate = alsa_track->rate;

    /* frame counts are always the same to both read and write */
    while (alsa_track->keep_thread_running) {

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
            for (i = 0; i < alsa_track->periods; i += 1)
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

        gnuitar_mutex_lock(&track->pump_mutex);
        gnuitar_pump_process(track->pump, &db);
        gnuitar_mutex_unlock(&track->pump_mutex);

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
    
    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0)
	return 1;
    
    if ((err = snd_pcm_hw_params_any(device, hw_params)) < 0) {
        snd_pcm_hw_params_free(hw_params);
	return 1;
    }

    if ((err = snd_pcm_hw_params_set_access(device, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        snd_pcm_hw_params_free(hw_params);
	return 1;
    }

    /* XXX we should support floating-point sample format */
    if ((err = snd_pcm_hw_params_set_format(device, hw_params, SND_PCM_FORMAT_FLOAT)) < 0) {
        if ((err = snd_pcm_hw_params_set_format(device, hw_params, SND_PCM_FORMAT_S16)) < 0) {
            snd_pcm_hw_params_free(hw_params);
	    return 1;
        } else {
            *bits = 16;
        }
    } else {
        *bits = 32;
    }

    if ((err = snd_pcm_hw_params_set_channels(device, hw_params, channels)) < 0) {
        snd_pcm_hw_params_free(hw_params);
        return 1;
    }

    /* ALSA has poor quality resampling. User is much better off without
     * resampling and GNUitar adapted to proper frequency than with it. */
#if(SND_LIB_MAJOR >=1 && SND_LIB_MINOR >= 0 && SND_LIB_SUBMINOR >8)
    if ((err = snd_pcm_hw_params_set_rate_resample(device, hw_params, 0)) < 0) {
        /* this isn't fatal, though. We'll just continue. */
    }
#endif

    if (adapting) {
        /* Adapting path: choose values for the tunables:
         * sampling rate, fragment size, number of fragments. */
        tmp = *rate;
        if ((err = snd_pcm_hw_params_set_rate_near(device, hw_params, &tmp, 0)) < 0) {
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        if (tmp != *rate) {
            *rate = tmp;
        }

        /* tell alsa how many periods we would like to have */
        if ((err = snd_pcm_hw_params_set_periods(device, hw_params, *fragments, 0)) < 0) {
            /* If it doesn't work, well, no matter. Next time *fragments will have
             * another value which hopefully works. And set_buffer_time_near() may
             * still work. The message to take home is: ALSA is difficult to program. */
        }

        /* let the sound track pick period size as appropriate. */
        tmp = (float) (*frames * *fragments) / (*rate) * 1E6;

        if ((err = snd_pcm_hw_params_set_buffer_time_near(device, hw_params, &tmp, NULL)) < 0) {
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        /* obtain the frames and fragments chosen by ALSA */
        if ((err = snd_pcm_hw_params_get_period_size(hw_params, &alsa_frames, NULL)) < 0) {
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        *frames = alsa_frames;
        if ((err = snd_pcm_hw_params_get_periods(hw_params, fragments, NULL)) < 0) {
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
    } else {
        /* use the fixed values given before */
        tmp = *rate;
        if ((err = snd_pcm_hw_params_set_rate(device, hw_params, tmp, 0)) < 0) {
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        if ((err = snd_pcm_hw_params_set_period_size(device, hw_params, *frames, 0)) < 0) {
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }
        
        if ((err = snd_pcm_hw_params_set_periods(device, hw_params, *fragments, 0)) < 0) {
            snd_pcm_hw_params_free(hw_params);
            return 1;
        }        
    }

    if ((err = snd_pcm_hw_params(device, hw_params)) < 0) {
        snd_pcm_hw_params_free(hw_params);
        return 1;
    }

    snd_pcm_hw_params_free(hw_params);

    return 0;
}

