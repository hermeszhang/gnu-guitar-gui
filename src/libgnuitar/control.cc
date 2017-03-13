#include <libgnuitar/control.h>

namespace Gnuitar
{

Control::Control (void) noexcept
{
  value = 0.0f;
  max = 1.0f;
  min = -1.0f;
}

Control::~Control (void)
{

}

float
Control::get_max (void) const noexcept
{
  return max;
}

float
Control::get_min (void) const noexcept
{
  return min;
}

float
Control::get_value (void) const noexcept
{
  return value;
}

const std::string&
Control::get_label (void) const noexcept
{
  return label;
}

void
Control::set_max (float max_) noexcept
{
  max = max_;
  if (value > max)
    value = max;
  if (max < min)
    min = max;
}

void
Control::set_min (float min_) noexcept
{
  min = min_;
  if (min > value)
    value = min;
  if (min > max)
    max = min;
}

void
Control::set_value (float value_) noexcept
{
  value = value_;
  if (value > max)
    value = max;
  else if (value < min)
    value = min;
}

void
Control::set_label (const std::string& label_)
{
  label = label_;
}

} /* namespace Gnuitar */

