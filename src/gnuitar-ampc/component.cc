#include <gnuitar-ampc/component.h>

namespace Gnuitar
{

namespace AmpC
{

Component::Component (void) noexcept
{

}

Component::~Component (void)
{

}

long
Component::get_input (void) const noexcept
{
  return input;
}

long
Component::get_output (void) const noexcept
{
  return output;
}

const std::string&
Component::get_label (void) const noexcept
{
  return label;
}

void
Component::set_input (long input_) noexcept
{
  input = input_;
}

void
Component::set_output (long output_) noexcept
{
  output = output_;
}

void
Component::set_label (const std::string& label_) noexcept
{
  label = label_;
}

void
Component::set_label (std::string&& label_) noexcept
{
  label = label_;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

