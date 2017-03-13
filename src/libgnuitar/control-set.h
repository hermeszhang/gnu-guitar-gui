#ifndef GNUITAR_CONTROL_SET_H
#define GNUITAR_CONTROL_SET_H

#include <libgnuitar/control.h>

#include <vector>

namespace Gnuitar
{

class ControlSet final
{
  std::vector<Control> controls;
public:
  ControlSet (void) noexcept;
  ~ControlSet (void);
  void add (Control&& control);
  void set (const std::string& name, float value);
  float get (const std::string& name);
  std::vector<Control>::const_iterator begin (void) const;
  std::vector<Control>::const_iterator end (void) const;
}; /* class ControlSet */

} /* namespace Gnuitar */

#endif /* GNUITAR_CONTROL_SET_H */

