#include <libgnuitar/alsa-driver.h>

#include <libgnuitar/exception.h>

namespace Gnuitar
{

AlsaDriver::AlsaDriver (void) noexcept
{
  input_pcm = NULL;
  output_pcm = NULL;
  keep_thread_running = false;
  channels = 2;
  rate = 48000;
}

AlsaDriver::~AlsaDriver (void)
{
  stop();
  if (input_pcm)
    snd_pcm_close (input_pcm);
  if (output_pcm)
    snd_pcm_close (output_pcm);
}

size_t
AlsaDriver::get_channels (void) const
{
  return channels;
}

size_t
AlsaDriver::get_rate (void) const
{
  return rate;
}

bool
AlsaDriver::running (void) const noexcept
{
  return keep_thread_running;
}

void
AlsaDriver::set_input (const std::string& input_name)
{
  snd_pcm_t *tmp_pcm;

  auto err = snd_pcm_open (&tmp_pcm, input_name.c_str(), SND_PCM_STREAM_CAPTURE, 0);
  if (err != 0)
    throw SystemException ("failed to open capture PCM", err);

  configure_pcm (tmp_pcm);

  mutex.lock ();
  if (input_pcm)
    snd_pcm_close (input_pcm);
  input_pcm = tmp_pcm;
  mutex.unlock ();
}

void
AlsaDriver::set_output (const std::string& output_name)
{
  snd_pcm_t *tmp_pcm;

  auto err = snd_pcm_open (&tmp_pcm, output_name.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
  if (err != 0)
    throw SystemException ("failed to open playback PCM", err);

  configure_pcm (tmp_pcm);

  mutex.lock();
  if (output_pcm)
    snd_pcm_close (output_pcm);
  output_pcm = tmp_pcm;
  mutex.unlock ();
}

void
AlsaDriver::start (void)
{
  if (running())
    return;

  if (input_pcm == nullptr)
    set_input ("default");

  if (output_pcm == nullptr)
    set_output ("default");

  keep_thread_running = true;

  thread = std::thread (&AlsaDriver::loop, this);
}

void
AlsaDriver::stop (void)
{
  if (running())
    {
      keep_thread_running = false;
      if (thread.joinable ())
        thread.join ();
    }
}

void
AlsaDriver::loop (void)
{
  float *sample_array;
  size_t frame_count = 256;

  sample_array = new float[frame_count * channels];

  connect (sample_array);

  while (keep_thread_running)
    {
      mutex.lock ();

      auto read_count = snd_pcm_readi (input_pcm, sample_array, frame_count);
      if (read_count < 0)
        {
          recover_pcm (input_pcm);
          read_count = 0;
        }

      run (read_count * channels);

      auto write_count = snd_pcm_writei (output_pcm, sample_array, read_count);
      if (write_count < 0)
        {
          recover_pcm (output_pcm);
          write_count = 0;
        }

      mutex.unlock ();
    }

  delete [] sample_array;
}

void
AlsaDriver::recover_pcm (snd_pcm_t *pcm)
{
  auto err = snd_pcm_drop (pcm);
  if (err != 0)
    throw SystemException ("failed to drop PCM", err);
  err = snd_pcm_prepare (pcm);
  if (err != 0)
    throw SystemException ("failed to prepare PCM", err);
}

void
AlsaDriver::configure_pcm (snd_pcm_t *pcm)
{
  snd_pcm_hw_params_t *hw_params;
  int err;

  struct
  {
    unsigned int period_size;
    unsigned int periods;
    snd_pcm_format_t format;
  } pcm_config;

  pcm_config.period_size = 128;
  pcm_config.periods = 4;
  pcm_config.format = SND_PCM_FORMAT_FLOAT;

  err = snd_pcm_hw_params_malloc (&hw_params);
  if (err < 0)
    throw SystemException ("failed to allocate PCM hardware parameters", err);

  err = snd_pcm_hw_params_any (pcm, hw_params);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      throw SystemException ("failed to get PCM hardware parameter space", err);
    }

  err = snd_pcm_hw_params_set_access (pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      throw SystemException ("failed to set PCM access", err);
    }

  err = snd_pcm_hw_params_set_format (pcm, hw_params, pcm_config.format);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      throw SystemException ("failed to set PCM format", err);
    }

  err = snd_pcm_hw_params_set_channels (pcm, hw_params, channels);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      throw SystemException ("failed to set PCM channel count", err);
    }

    /* ALSA has poor quality resampling. User is much better off without
     * resampling and GNUitar adapted to proper frequency than with it. */
#if(SND_LIB_MAJOR >=1 && SND_LIB_MINOR >= 0 && SND_LIB_SUBMINOR >8)
  snd_pcm_hw_params_set_rate_resample(pcm, hw_params, 0);
#endif

  err = snd_pcm_hw_params_set_rate_near (pcm, hw_params, &rate, 0);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      throw SystemException ("failed to set PCM rate", err);
    }

  /* tell alsa how many periods we would like to have */
  snd_pcm_hw_params_set_periods (pcm, hw_params, pcm_config.periods, 0);
  /* If it doesn't work, well, no matter. Next time *fragments will have
   * another value which hopefully works. And set_buffer_time_near() may
   * still work. The message to take home is: ALSA is difficult to program. */

  err = snd_pcm_hw_params_set_period_size (pcm, hw_params, pcm_config.period_size, 0);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      throw SystemException ("failed to set PCM period size", err);
    }
        
  err = snd_pcm_hw_params (pcm, hw_params);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      throw SystemException ("failed to set PCM hardware parameters", err);
    }

  snd_pcm_hw_params_free (hw_params);
}

} /* namespace Gnuitar */

