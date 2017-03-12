#include <gnuitar-ampc/circuit.h>

#include <gnuitar-ampc/exception.h>

namespace Gnuitar
{

namespace AmpC
{

Circuit::Circuit (void) noexcept
{

}

Circuit::~Circuit (void)
{
  for (Component *component : components)
    delete component;
}

void
Circuit::accept (Visitor& visitor) const noexcept
{
  for (const Component* component : components)
    component->accept(visitor);
}

void
Circuit::add (const Capacitor& capacitor)
{
  return add (new Capacitor(capacitor));
}

void
Circuit::add (const Resistor& resistor)
{
  return add (new Resistor(resistor));
}

void
Circuit::add (Component *component) noexcept
{
  if (component != nullptr)
    components.push_back(component);
}

} /* namespace AmpC */

} /* namespace Gnuitar */

