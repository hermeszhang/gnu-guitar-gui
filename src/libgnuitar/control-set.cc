#include <libgnuitar/control-set.h>

#include <libgnuitar/exception.h>

namespace Gnuitar
{

ControlSet::ControlSet (void) noexcept
{

}

ControlSet::~ControlSet (void)
{

}

void
ControlSet::add (Control&& control)
{
  controls.emplace_back (control);
}

void
ControlSet::set (const std::string& label, float value)
{
  for (auto& control : controls)
    {
      if (control.get_label () == label)
        {
          control.set_value (value);
	  return;
        }
    }
  throw Exception("control does not exist");
}

std::vector<Control>::const_iterator
ControlSet::begin (void) const
{
  return controls.begin ();
}

std::vector<Control>::const_iterator
ControlSet::end (void) const
{
  return controls.end ();
}

} /* namespace Gnuitar */

