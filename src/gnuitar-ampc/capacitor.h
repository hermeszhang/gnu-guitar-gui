#ifndef GNUITAR_AMPC_CAPACITOR_H
#define GNUITAR_AMPC_CAPACITOR_H

#include <gnuitar-ampc/component.h>

namespace Gnuitar
{

namespace AmpC
{

class Capacitor final : public Component
{
  float capacitance;
  float charge;
public:
  Capacitor (void) noexcept;
  Capacitor (Component&& component);
  Capacitor (Capacitor&& capacitor);
  ~Capacitor (void);
  void accept (Visitor& visitor) const noexcept;
  void set_capacitance (float capacitance_) noexcept;
  float get_capacitance (void) const noexcept;
}; /* class Capacitor */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_CAPACITOR_H */

