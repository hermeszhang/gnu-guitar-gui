#include <libgnuitar/alsa-driver.h>

#ifdef GNUITAR_DEBUG
#include <iostream>
#endif /* GNUITAR_DEBUG */

namespace Gnuitar
{

namespace ALSA
{

Driver::Driver (void) noexcept
{
  input_pcm = NULL;
  output_pcm = NULL;
  keep_thread_running = false;
  channels = 2;
  rate = 48000;
}

Driver::~Driver (void)
{
  stop();
  if (input_pcm)
    snd_pcm_close (input_pcm);
  if (output_pcm)
    snd_pcm_close (output_pcm);
}

size_t
Driver::get_channels (void) const noexcept
{
  return channels;
}

size_t
Driver::get_rate (void) const noexcept
{
  return rate;
}

bool
Driver::running (void) const noexcept
{
  return keep_thread_running;
}

int
Driver::set_input (const std::string& input_name) noexcept
{
  snd_pcm_t *tmp_pcm;

  if (snd_pcm_open (&tmp_pcm, input_name.c_str(), SND_PCM_STREAM_CAPTURE, 0) != 0)
    return EINVAL;

  if (configure_pcm (tmp_pcm) != 0)
    {
#ifdef GNUITAR_DEBUG
      std::cerr << "failed to configure " << input_name << std::endl;
#endif /* GNUITAR_DEBUG */
      snd_pcm_close (tmp_pcm);
      return EINVAL;
    }

  mutex.lock ();

  if (input_pcm)
    snd_pcm_close (input_pcm);

  input_pcm = tmp_pcm;

  mutex.unlock ();

  return 0;
}

int
Driver::set_output (const std::string& output_name) noexcept
{
  snd_pcm_t *tmp_pcm;

  if (snd_pcm_open (&tmp_pcm, output_name.c_str(), SND_PCM_STREAM_PLAYBACK, 0) != 0)
    return EINVAL;

  if (configure_pcm (tmp_pcm) != 0)
    {
#ifdef GNUITAR_DEBUG
      std::cerr << "failed to configure " << output_name << std::endl;
#endif /* GNUITAR_DEBUG */
      snd_pcm_close (tmp_pcm);
      return EINVAL;
    }

  mutex.lock();

  if (output_pcm)
    snd_pcm_close (output_pcm);

  output_pcm = tmp_pcm;

  mutex.unlock ();

  return 0;
}

int
Driver::start (void) noexcept
{
  if (running())
    return 0;

  if (input_pcm == nullptr)
    if (set_input ("default") != 0)
      return EINVAL;
  if (output_pcm == nullptr)
    if (set_output ("default") != 0)
      return EINVAL;

  keep_thread_running = true;
  thread = std::thread (&Driver::loop, this);
  return 0;
}

int
Driver::stop (void) noexcept
{
  if (!running())
    return 0;

  keep_thread_running = false;
  if (thread.joinable ())
    thread.join ();
  return 0;
}

int
Driver::loop (void) noexcept
{
  float *sample_array;
  size_t frame_count = 128;

  sample_array = new float[frame_count * channels];

  connect(sample_array);

  while (keep_thread_running)
    {
      mutex.lock ();

      auto read_count = snd_pcm_readi (input_pcm, sample_array, frame_count);
      if (read_count < 0)
        {
          if (recover_pcm(input_pcm) != 0)
            return EPIPE;
          read_count = 0;
        }

      run(read_count * channels);

      auto write_count = snd_pcm_writei (output_pcm, sample_array, read_count);
      if (write_count < 0)
        {
          if (recover_pcm(output_pcm) != 0)
            return EPIPE;
          write_count = 0;
        }

      mutex.unlock ();
    }

  delete [] sample_array;

  return 0;
}

int
Driver::recover_pcm (snd_pcm_t *pcm) noexcept
{
  int err;
  err = snd_pcm_drop(pcm);
  if (err != 0)
    return err;
  err = snd_pcm_prepare(pcm);
  if (err != 0)
    return err;
  return 0;
}

int
Driver::configure_pcm (snd_pcm_t *pcm) noexcept
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
    return 1;
    
  err = snd_pcm_hw_params_any (pcm, hw_params);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      return 1;
    }

  err = snd_pcm_hw_params_set_access (pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      return 1;
    }

  err = snd_pcm_hw_params_set_format (pcm, hw_params, pcm_config.format);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      return 1;
    }

  err = snd_pcm_hw_params_set_channels (pcm, hw_params, channels);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      return 1;
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
      return 1;
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
      return 1;
    }
        
  err = snd_pcm_hw_params (pcm, hw_params);
  if (err < 0)
    {
      snd_pcm_hw_params_free (hw_params);
      return 1;
    }

  snd_pcm_hw_params_free (hw_params);

  return 0;
}

} /* namespace ALSA */

} /* namespace Gnuitar */

