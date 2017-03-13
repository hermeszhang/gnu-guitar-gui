#ifndef GNUITAR_CONTROL_H
#define GNUITAR_CONTROL_H

#include <string>

namespace Gnuitar
{

class Control final
{
  float max;
  float min;
  float value;
  std::string label;
public:
  Control (void) noexcept;
  ~Control (void);
  float get_max (void) const noexcept;
  float get_min (void) const noexcept;
  float get_value (void) const noexcept;
  const std::string& get_label (void) const noexcept;
  void set_max (float max_) noexcept;
  void set_min (float min_) noexcept;
  void set_value (float value_) noexcept;
  void set_label (const std::string& label_);
}; /* class Control */

} /* namespace Gnuitar */

#endif /* GNUITAR_CONTROL_H */

