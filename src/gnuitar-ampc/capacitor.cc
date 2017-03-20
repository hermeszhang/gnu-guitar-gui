#include <gnuitar-ampc/capacitor.h>

#include <gnuitar-ampc/visitor.h>

namespace Gnuitar
{

namespace AmpC
{

Capacitor::Capacitor (void) noexcept
{

}

Capacitor::Capacitor (Component&& component) : Component (std::move (component))
{

}

Capacitor::Capacitor (Capacitor&& capacitor) : Component (std::move (capacitor))
{
  capacitance = capacitor.capacitance;
}

Capacitor::~Capacitor (void)
{

}

void
Capacitor::accept (Visitor& visitor) const noexcept
{
  visitor.visit(*this);
}

float
Capacitor::get_capacitance (void) const noexcept
{
  return capacitance;
}

void
Capacitor::set_capacitance (float capacitance_) noexcept
{
  capacitance = capacitance_;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

