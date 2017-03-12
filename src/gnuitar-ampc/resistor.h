#ifndef GNUITAR_AMPC_RESISTOR_H
#define GNUITAR_AMPC_RESISTOR_H

#include <gnuitar-ampc/component.h>

namespace Gnuitar
{

namespace AmpC
{

class Resistor final : public Component
{
  float resistance;
public:
  Resistor (void) noexcept;
  ~Resistor (void);
  void accept(Visitor& visitor) const noexcept;
  float get_resistance (void) const noexcept;
  void set_resistance (float resistance_) noexcept;
}; /* class Resistor */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_RESISTOR_H */

