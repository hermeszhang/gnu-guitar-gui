#ifndef GNUITAR_ALSA_DRIVER_H
#define GNUITAR_ALSA_DRIVER_H

#include <thread>
#include <mutex>

#include <alsa/asoundlib.h>

#include <libgnuitar/driver.h>

namespace Gnuitar
{

namespace ALSA
{

class Driver final : public Gnuitar::Driver
{
  std::mutex mutex;
  std::thread thread;
  bool keep_thread_running;
  snd_pcm_t *input_pcm;
  snd_pcm_t *output_pcm;
public:
  Driver(void) noexcept;
  ~Driver(void);
  int set_input(const std::string& input_name) noexcept;
  int set_output(const std::string& output_name) noexcept;
  int start(void) noexcept;
  int stop(void) noexcept;
  bool running(void) const noexcept;
protected:
  int configure_pcm(snd_pcm_t *pcm) noexcept;
  int recover_pcm(snd_pcm_t *pcm) noexcept;
  int loop(void) noexcept;
}; /* class Driver */

} /* namespace ALSA */

} /* namespace Gnuitar */

#endif /* GNUITAR_ALSA_DRIVER_H */

