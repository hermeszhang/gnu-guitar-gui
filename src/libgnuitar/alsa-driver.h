#ifndef GNUITAR_ALSA_DRIVER_H
#define GNUITAR_ALSA_DRIVER_H

#include <thread>
#include <mutex>

#include <alsa/asoundlib.h>

#include <libgnuitar/driver.h>

namespace Gnuitar
{

class AlsaDriver final : public Gnuitar::Driver
{
  std::mutex mutex;
  std::thread thread;
  bool keep_thread_running;
  snd_pcm_t *input_pcm;
  snd_pcm_t *output_pcm;
  unsigned int channels;
  unsigned int rate;
public:
  AlsaDriver (void) noexcept;
  ~AlsaDriver (void);
  size_t get_channels (void) const;
  size_t get_rate (void) const;
  void set_input (const std::string& input_name);
  void set_output (const std::string& output_name);
  void start (void);
  void stop (void);
  bool running (void) const noexcept;
protected:
  void configure_pcm (snd_pcm_t *pcm);
  void recover_pcm (snd_pcm_t *pcm);
  void loop (void);
}; /* class Driver */

} /* namespace Gnuitar */

#endif /* GNUITAR_ALSA_DRIVER_H */

