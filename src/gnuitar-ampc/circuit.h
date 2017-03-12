#ifndef GNUITAR_AMPC_CIRCUIT_H
#define GNUITAR_AMPC_CIRCUIT_H

#include <gnuitar-ampc/resistor.h>
#include <gnuitar-ampc/capacitor.h>

#include <vector>

namespace Gnuitar
{

namespace AmpC
{

class Circuit final
{
  std::vector<Component*> components;
public:
  Circuit (void) noexcept;
  ~Circuit (void);
  void accept (Visitor& visitor) const noexcept;
  void add (const Resistor& resistor);
  void add (const Capacitor& capacitor);
protected:
  void add (Component *capacitor) noexcept;
}; /* class Circuit */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_CIRCUIT_H */

