#ifndef GNUITAR_EFFECT_H
#define GNUITAR_EFFECT_H

#include <string>

#include <libgnuitar/control-set.h>

#include "plugin-apis/ladspa.h"

namespace Gnuitar
{

class Effect final
{
  LADSPA_Handle handle;
  const LADSPA_Descriptor *descriptor;
  std::string name;
  ControlSet control_set;
public:
  Effect(const LADSPA_Descriptor *descriptor) noexcept;
  ~Effect(void);
  void instantiate (unsigned long rate);
  void activate (void);
  const std::string& get_name (void) const;
  const ControlSet& get_control_set (void) const;
  float get_control_value (const std::string& name) const;
  void set_control_value (const std::string& name, float value);
  void connect (float *sample_array);
  void run (size_t sample_count);
protected:
  void create_controls (void);
}; /* class Effect */

} /* namespace Gnuitar */

#endif /* GNUITAR_EFFECT_H */

