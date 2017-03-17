#ifndef GNUITAR_DRIVER_H
#define GNUITAR_DRIVER_H

#include <string>
#include <vector>

#include <libgnuitar/effect.h>

namespace Gnuitar
{

class Driver
{
  std::vector<Effect*> effects;
public:
  static Driver * make (void);
  virtual ~Driver (void);
  void add_effect (Effect *effect);
  Effect * get_effect (void);
  const Effect * get_effect (void) const;
  virtual size_t get_channels (void) const = 0;
  virtual size_t get_rate (void) const = 0;
  float get_control_value (const std::string& control_name);
  void set_control_value (const std::string& effect_name, const std::string& control_name, float value);
  virtual void set_input (const std::string& input_name) = 0;
  virtual void set_output (const std::string& output_name) = 0;
  virtual bool running (void) const = 0;
  virtual void start (void) = 0;
  virtual void stop (void) = 0;
protected:
  void connect (float *sample_array);
  void run (size_t sample_count);
}; /* class Driver */

} /* namespace Gnuitar */

#endif /* GNUITAR_DRIVER_H */

