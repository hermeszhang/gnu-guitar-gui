#ifndef GNUITAR_AMPC_COMPONENT_H
#define GNUITAR_AMPC_COMPONENT_H

#include <string>

namespace Gnuitar
{

namespace AmpC
{

class Component
{
  long input;
  long output;
  char designator;
  std::string label;
public:
  Component(void) noexcept;
  Component(Component&& component);
  virtual ~Component (void);
  virtual void accept(class Visitor& visitor) const noexcept;
  void set_input (long input) noexcept;
  void set_output (long output) noexcept;
  void set_label (const std::string& label_) noexcept;
  void set_label (std::string&& label_) noexcept;
  void set_designator (char designator_);
  long get_input (void) const noexcept;
  long get_output (void) const noexcept;
  char get_designator (void) const;
  const std::string& get_label (void) const noexcept;
}; /* class Component */

} /* namespace Component */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_COMPONENT_H */

