#ifndef GNUITAR_AMPC_COMPONENT_H
#define GNUITAR_AMPC_COMPONENT_H

#include <string>

namespace Gnuitar
{

namespace AmpC
{

class Component
{
private:
  long input;
  long output;
  std::string label;
public:
  Component(void) noexcept;
  virtual ~Component (void);
  virtual void accept(class Visitor& visitor) const noexcept = 0;
  void set_input (long input) noexcept;
  void set_output (long output) noexcept;
  void set_label (const std::string& label_) noexcept;
  void set_label (std::string&& label_) noexcept;
  long get_input (void) const noexcept;
  long get_output (void) const noexcept;
  const std::string& get_label (void) const noexcept;
}; /* class Component */

} /* namespace Component */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_COMPONENT_H */

