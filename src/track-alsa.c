#include "track.h"
#include "track-alsa.h"

#ifdef _MSC_VER
#define gnuitar_strdup _strdup
#else
#define gnuitar_strdup strdup
#endif

static void * create(void);

static void done(void *data);

static int start(struct GnuitarTrack *track);

static int stop(struct GnuitarTrack *track);

static int get_map(const struct GnuitarTrack *track, struct GnuitarMap *map);

static void * alsa_audio_thread(void *data);

static int alsa_configure_audio(snd_pcm_t *device, struct GnuitarPcmConfig *pcm_config);

int
gnuitar_alsa_track_init(struct GnuitarTrack *track)
{
    gnuitar_mutex_init(&track->chain_mutex);

    gnuitar_chain_init(&track->chain);

    track->done = done;
    track->start = start;
    track->stop = stop;
    track->get_map = get_map;

    track->data = create();
    if (track->data == NULL) {
        return ENOMEM;
    }

    return 0;
}

static void *
create(void)
{
    struct GnuitarAlsaTrack *alsa_track;

    alsa_track = malloc(sizeof(*alsa_track));
    if (alsa_track == NULL)
        return NULL;

    alsa_track->input_name = gnuitar_strdup("default");
    alsa_track->input_pcm = NULL;

    alsa_track->output_name = gnuitar_strdup("default");
    alsa_track->output_pcm = NULL;

    alsa_track->pcm_config.format = SND_PCM_FORMAT_S32;
    alsa_track->pcm_config.channels = 2;
    alsa_track->pcm_config.rate = 48000;
    alsa_track->pcm_config.periods = 2;
    alsa_track->pcm_config.period_size = 512;

    alsa_track->keep_thread_running = 0;

    return alsa_track;
}

static void
done(void *data)
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
start(struct GnuitarTrack *track)
{
    struct GnuitarAlsaTrack *alsa_track;

    alsa_track = (struct GnuitarAlsaTrack *)(track->data);
    if (alsa_track == NULL)
        return -1;

    if (snd_pcm_open(&alsa_track->input_pcm, alsa_track->input_name, SND_PCM_STREAM_CAPTURE, 0 /* mode flags */) != 0) {
        return -2;
    }

    if (alsa_configure_audio(alsa_track->input_pcm, &alsa_track->pcm_config) != 0){
        return -3;
    }

    if (snd_pcm_open(&alsa_track->output_pcm, alsa_track->output_name, SND_PCM_STREAM_PLAYBACK, 0 /* mode flags */) != 0) {
        return -4;
    }

    if (alsa_configure_audio(alsa_track->output_pcm, &alsa_track->pcm_config) != 0){
        return -5;
    }

    alsa_track->keep_thread_running = 1;

    if (pthread_create(&alsa_track->thread, NULL, alsa_audio_thread, track)) {
        return -6;
    }

    return 0;
}

static int
stop(struct GnuitarTrack *track)
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

static int
get_map(const struct GnuitarTrack *track, struct GnuitarMap *map)
{
    int err;

    (void) track;

    err = gnuitar_map_define(map, "Rate", GNUITAR_MAP_TYPE_UINT32);
    if (err)
        return err;

    err = gnuitar_map_define(map, "Periods", GNUITAR_MAP_TYPE_UINT32);
    if (err)
        return err;

    err = gnuitar_map_define(map, "Period Size", GNUITAR_MAP_TYPE_UINT32);
    if (err)
        return err;

    err = gnuitar_map_define(map, "Input Device", GNUITAR_MAP_TYPE_STRING);
    if (err)
        return err;

    err = gnuitar_map_define(map, "Output Device", GNUITAR_MAP_TYPE_STRING);
    if (err)
        return err;

    return 0;
}

static void *
alsa_audio_thread(void *data)
{
    unsigned int i;
    int inframes, outframes;
    struct GnuitarPacket db;
    unsigned int restarting = 0;
    ssize_t data_size;

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

    data_size = snd_pcm_frames_to_bytes(alsa_track->input_pcm, alsa_track->pcm_config.period_size);
    if (data_size < 0)
        return NULL;

    db.data = malloc(data_size);
    if (db.data == NULL)
        return NULL;

    db.data_swap = malloc(data_size);
    if (db.data_swap == NULL) {
        free(db.data);
        return NULL;
    }

    db.len = alsa_track->pcm_config.period_size * alsa_track->pcm_config.channels;
    db.channels = alsa_track->pcm_config.channels;
    db.rate = alsa_track->pcm_config.rate;

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
            for (i = 0; i < alsa_track->pcm_config.periods; i++)
                if (snd_pcm_avail_update(output_pcm) > 0)
                    snd_pcm_writei(output_pcm, db.data, db.len / db.channels);
        }
        while ((inframes = snd_pcm_readi(input_pcm, db.data, db.len / db.channels)) < 0) {
            restarting = 1;
            snd_pcm_prepare(input_pcm);
        }
        db.len = inframes * db.channels;
        gnuitar_mutex_lock(&track->chain_mutex);
        gnuitar_chain_process(&track->chain, &db);
        gnuitar_mutex_unlock(&track->chain_mutex);

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
alsa_configure_audio(snd_pcm_t *device, struct GnuitarPcmConfig *pcm_config)
{
    snd_pcm_hw_params_t *hw_params;
    int err;

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

    if ((err = snd_pcm_hw_params_set_format(device, hw_params, pcm_config->format)) < 0) {
        return 1;
    }

    if ((err = snd_pcm_hw_params_set_channels(device, hw_params, pcm_config->channels)) < 0) {
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

    if ((err = snd_pcm_hw_params_set_rate_near(device, hw_params, &pcm_config->rate, 0)) < 0) {
        snd_pcm_hw_params_free(hw_params);
        return 1;
    }

    /* tell alsa how many periods we would like to have */
    if ((err = snd_pcm_hw_params_set_periods(device, hw_params, pcm_config->periods, 0)) < 0) {
        /* If it doesn't work, well, no matter. Next time *fragments will have
         * another value which hopefully works. And set_buffer_time_near() may
         * still work. The message to take home is: ALSA is difficult to program. */
    }

    if ((err = snd_pcm_hw_params_set_period_size(device, hw_params, pcm_config->period_size, 0)) < 0) {
        snd_pcm_hw_params_free(hw_params);
        return 1;
    }
        
    if ((err = snd_pcm_hw_params(device, hw_params)) < 0) {
        snd_pcm_hw_params_free(hw_params);
        return 1;
    }

    snd_pcm_hw_params_free(hw_params);

    return 0;
}

