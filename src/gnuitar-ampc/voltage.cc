#include <gnuitar-ampc/voltage.h>

#include <gnuitar-ampc/visitor.h>

namespace Gnuitar
{

namespace AmpC
{

Voltage::Voltage (void)
{
  voltage = 1.0f;
}

Voltage::Voltage (Component&& component) : Component (std::move (component))
{

}

Voltage::Voltage (Voltage&& voltage_) : Component (std::move (voltage_))
{
  voltage = voltage_.voltage;
}

Voltage::~Voltage (void)
{

}

void
Voltage::accept (Visitor& visitor) const noexcept
{
  visitor.visit (*this);
}

float
Voltage::get_voltage (void) const
{
  return voltage;
}

void
Voltage::set_voltage (float voltage_)
{
  voltage = voltage_;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

