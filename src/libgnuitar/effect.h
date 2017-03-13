#ifndef GNUITAR_EFFECT_H
#define GNUITAR_EFFECT_H

#include <string>
#include <vector>

#include "plugin-apis/ladspa.h"

namespace Gnuitar
{

class Effect final
{
  LADSPA_Handle handle;
  const LADSPA_Descriptor *descriptor;
public:
  Effect(const LADSPA_Descriptor *descriptor) noexcept;
  ~Effect(void);
  bool instantiate (unsigned long rate) noexcept;
  bool activate (void) noexcept;
  bool get_name (std::string& name) const noexcept;
  std::vector<std::string> get_control_names (void) const;
  bool get_control_value (const std::string& name, float *value) const noexcept;
  bool set_control_value (const std::string& name, float *value) noexcept;
  bool connect (float *sample_array) noexcept;
  bool run (size_t sample_count) noexcept;
}; /* class Effect */

} /* namespace Gnuitar */

#endif /* GNUITAR_EFFECT_H */

