#ifndef GNUITAR_DRIVER_H
#define GNUITAR_DRIVER_H

#include "plugin.h"

#include <string>
#include <vector>

namespace Gnuitar
{

class Driver
{
  std::vector<Effect*> effects;
public:
  static Driver * make (void) noexcept;
  virtual ~Driver(void);
  int add_effect(Effect *effect) noexcept;
  virtual int set_input(const std::string& input_name) noexcept = 0;
  virtual int set_output(const std::string& output_name) noexcept = 0;
  virtual bool running (void) const noexcept = 0;
  virtual int start (void) noexcept = 0;
  virtual int stop (void) noexcept = 0;
protected:
  bool connect(float *sample_array) noexcept;
  void run(size_t sample_count) noexcept;
}; /* class Driver */

} /* namespace Gnuitar */

#endif /* GNUITAR_DRIVER_H */

