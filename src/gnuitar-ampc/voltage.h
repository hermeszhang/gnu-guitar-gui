#ifndef GNUITAR_AMPC_VOLTAGE_H
#define GNUITAR_AMPC_VOLTAGE_H

#include <gnuitar-ampc/component.h>

namespace Gnuitar
{

namespace AmpC
{

class Voltage final : public Component
{
  float voltage;
public:
  Voltage (void);
  Voltage (Component&& component);
  Voltage (Voltage&& voltage);
  ~Voltage (void);
  void accept (Visitor& visitor) const noexcept;
  float get_voltage (void) const;
  void set_voltage (float voltage_);
}; /* class Voltage */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_VOLTAGE_H */

