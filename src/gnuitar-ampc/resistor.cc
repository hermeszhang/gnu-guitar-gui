#include <gnuitar-ampc/resistor.h>

#include <gnuitar-ampc/visitor.h>

namespace Gnuitar
{

namespace AmpC
{

Resistor::Resistor (void) noexcept
{

}

Resistor::~Resistor (void)
{

}

void
Resistor::accept (Visitor& visitor) const noexcept
{
  visitor.visit(*this);
}

float
Resistor::get_resistance (void) const noexcept
{
  return resistance;
}

void
Resistor::set_resistance (float resistance_) noexcept
{
  resistance = resistance_;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

