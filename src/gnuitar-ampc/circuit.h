#ifndef GNUITAR_AMPC_CIRCUIT_H
#define GNUITAR_AMPC_CIRCUIT_H

#include <gnuitar-ampc/resistor.h>
#include <gnuitar-ampc/capacitor.h>
#include <gnuitar-ampc/voltage.h>

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
  void accept (Visitor& visitor) const;
  void add (Resistor&& resistor);
  void add (Capacitor&& capacitor);
  void add (Voltage&& voltage);
protected:
  void add (Component *capacitor) noexcept;
  const Component * find_connected (const Component *source) const noexcept;
}; /* class Circuit */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_CIRCUIT_H */

