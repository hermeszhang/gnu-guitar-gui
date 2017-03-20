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
Circuit::accept (Visitor& visitor) const
{
  const Component *start = nullptr;

  /* find a non-null component */
  for (const auto it : components)
    {
      if (it != nullptr)
        {
          start = it;
          break;
        }
    }

  auto component = start;
  while (component != nullptr)
    {
      component->accept (visitor);
      component = find_connected (component);
      if (component == start)
        break;
      else if (component == nullptr)
        throw Exception ("component not connected");
    }
}

void
Circuit::add (Capacitor&& capacitor)
{
  return add (new Capacitor(std::move (capacitor)));
}

void
Circuit::add (Resistor&& resistor)
{
  return add (new Resistor(std::move (resistor)));
}

void
Circuit::add (Voltage&& voltage)
{
  return add (new Voltage(std::move (voltage)));
}

void
Circuit::add (Component *component) noexcept
{
  if (component != nullptr)
    components.push_back(component);
}

const Component *
Circuit::find_connected (const Component *component) const noexcept
{
  if (component == nullptr)
    return nullptr;

  auto output = component->get_output ();

  for (auto it : components)
    {
      if (it == nullptr)
        continue;
      if (it->get_input () == output)
        return it;
    }
  return nullptr;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

