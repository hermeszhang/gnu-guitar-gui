#include "../driver.h"
#include "driver-alsa.h"

#ifdef _MSC_VER
#define gnuitar_strdup _strdup
#else
#define gnuitar_strdup strdup
#endif

static void * create(void);

static void done(void *data);

static int start(void *data, struct GnuitarChain *chain);

static int stop(void *data);

static int get_map(const void *data, struct GnuitarMap *map);

static void * alsa_audio_thread(void *data);

static int alsa_configure_audio(snd_pcm_t *device, struct GnuitarPcmConfig *pcm_config);

int
gnuitar_alsa_driver_init(struct GnuitarDriver *driver)
{
    driver->done = done;
    driver->start = start;
    driver->stop = stop;
    driver->get_map = get_map;

    driver->data = create();
    if (driver->data == NULL) {
        return ENOMEM;
    }

    return 0;
}

static void *
create(void)
{
    struct GnuitarAlsaDriver *alsa_driver;

    alsa_driver = malloc(sizeof(*alsa_driver));
    if (alsa_driver == NULL)
        return NULL;

    alsa_driver->input_name = gnuitar_strdup("default");
    alsa_driver->input_pcm = NULL;

    alsa_driver->output_name = gnuitar_strdup("default");
    alsa_driver->output_pcm = NULL;

    alsa_driver->pcm_config.format = SND_PCM_FORMAT_S32;
    alsa_driver->pcm_config.channels = 2;
    alsa_driver->pcm_config.rate = 48000;
    alsa_driver->pcm_config.periods = 2;
    alsa_driver->pcm_config.period_size = 512;

    alsa_driver->chain = NULL;

    alsa_driver->keep_thread_running = 0;

    return alsa_driver;
}

static void
done(void *data)
{
    struct GnuitarAlsaDriver *alsa_driver;
    alsa_driver = (struct GnuitarAlsaDriver *)(data);
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
    free(data);
}

static int
start(void *data, struct GnuitarChain *chain)
{
    struct GnuitarAlsaDriver *alsa_driver;

    alsa_driver = (struct GnuitarAlsaDriver *)(data);
    if (alsa_driver == NULL)
        return -1;

    alsa_driver->chain = chain;

    if (snd_pcm_open(&alsa_driver->input_pcm, alsa_driver->input_name, SND_PCM_STREAM_CAPTURE, 0 /* mode flags */) != 0) {
        return -2;
    }

    if (alsa_configure_audio(alsa_driver->input_pcm, &alsa_driver->pcm_config) != 0){
        return -3;
    }

    if (snd_pcm_open(&alsa_driver->output_pcm, alsa_driver->output_name, SND_PCM_STREAM_PLAYBACK, 0 /* mode flags */) != 0) {
        return -4;
    }

    if (alsa_configure_audio(alsa_driver->output_pcm, &alsa_driver->pcm_config) != 0){
        return -5;
    }

    alsa_driver->keep_thread_running = 1;

    if (pthread_create(&alsa_driver->thread, NULL, alsa_audio_thread, data)) {
        return -6;
    }

    return 0;
}

static int
stop(void *data)
{
    struct GnuitarAlsaDriver *alsa_driver;

    alsa_driver = (struct GnuitarAlsaDriver *)(data);
    if (alsa_driver == NULL)
        return -1;

    alsa_driver->keep_thread_running = 0;

    if (pthread_join(alsa_driver->thread, NULL) != 0)
        return -1;

    alsa_driver->chain = NULL;

    return 0;
}

static int
get_map(const void *data, struct GnuitarMap *map)
{
    int err;

    (void) data;

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

    struct GnuitarAlsaDriver *alsa_driver;

    snd_pcm_t * input_pcm;
    snd_pcm_t * output_pcm;

    alsa_driver = (struct GnuitarAlsaDriver *)(data);
    if (alsa_driver == NULL)
        return NULL;

    input_pcm = alsa_driver->input_pcm;
    if (input_pcm == NULL)
        return NULL;

    output_pcm = alsa_driver->output_pcm;
    if (output_pcm == NULL)
        return NULL;

    data_size = snd_pcm_frames_to_bytes(alsa_driver->input_pcm, alsa_driver->pcm_config.period_size);
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

    db.len = alsa_driver->pcm_config.period_size * alsa_driver->pcm_config.channels;
    db.channels = alsa_driver->pcm_config.channels;
    db.rate = alsa_driver->pcm_config.rate;

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
            for (i = 0; i < alsa_driver->pcm_config.periods; i++)
                if (snd_pcm_avail_update(output_pcm) > 0)
                    snd_pcm_writei(output_pcm, db.data, db.len / db.channels);
        }
        while ((inframes = snd_pcm_readi(input_pcm, db.data, db.len / db.channels)) < 0) {
            restarting = 1;
            snd_pcm_prepare(input_pcm);
        }
        db.len = inframes * db.channels;
        gnuitar_chain_lock(alsa_driver->chain);
        gnuitar_chain_process(alsa_driver->chain, &db);
        gnuitar_chain_unlock(alsa_driver->chain);

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

