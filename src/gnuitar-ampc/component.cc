#include <gnuitar-ampc/component.h>

#include <gnuitar-ampc/visitor.h>

#ifndef GNUITAR_RELEASE
#include <iostream>
#endif /* GNUITAR_RELEASE */

namespace Gnuitar
{

namespace AmpC
{

Component::Component (void) noexcept
{

}

Component::Component (Component&& other)
{
  designator = other.designator;
  label = std::move (other.label);
  input = other.input;
  output = other.output;
}

Component::~Component (void)
{

}

void
Component::accept (Visitor&) const noexcept
{
#ifndef GNUITAR_RELEASE
  std::cerr << "gnuitar: warning:";
  std::cerr << "Gnuitar::AmpC::Component::accept was called";
  std::cerr << std::endl;
#endif /* GNUITAR_RELEASE */
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

char
Component::get_designator (void) const
{
  return designator;
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

void
Component::set_designator (char designator_)
{
  designator = designator_;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

