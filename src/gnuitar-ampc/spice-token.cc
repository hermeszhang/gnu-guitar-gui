#include "spice-token.h"

#include <cstdlib>

namespace Gnuitar
{

namespace AmpC
{

SpiceToken::SpiceToken (void) noexcept
{

}

SpiceToken::SpiceToken (const SpiceToken& spice_token) noexcept
{
  *this = spice_token;
}

SpiceToken::SpiceToken (SpiceToken&& spice_token) noexcept
{
  type = spice_token.type;
  data = std::move(spice_token.data);
}

SpiceToken::~SpiceToken (void)
{

}

SpiceToken& SpiceToken::operator = (const SpiceToken& spice_token) noexcept
{
  data = spice_token.data;
  type = spice_token.type;
  return *this;
}

void
SpiceToken::add_char (char c) noexcept
{
  data.push_back (c);
}

void
SpiceToken::clear (void) noexcept
{
  data.clear ();
}

const std::string&
SpiceToken::get_data (void) const noexcept
{
  return data;
}

bool
SpiceToken::get_integer (long *value) const noexcept
{
  if (type != SpiceToken::Type::integer)
    return false;
  else if (value == nullptr)
    return false;

  *value = std::strtol (data.c_str(), nullptr, 10);

  return true;
}

bool
SpiceToken::get_real_number (float *value) const noexcept
{
  if ((type != SpiceToken::Type::real_number)
   && (type != SpiceToken::Type::integer))
    return false;
  else if (value == nullptr)
    return false;

  *value = std::strtof (data.c_str(), nullptr);

  return true;
}

SpiceToken::Type
SpiceToken::get_type (void) const noexcept
{
  return type;
}

void
SpiceToken::set_type (SpiceToken::Type type_) noexcept
{
  type = type_;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

