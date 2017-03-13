#include "exception.h"

namespace Gnuitar
{

namespace AmpC
{

Exception::Exception (const char *what_msg) noexcept : std::runtime_error(what_msg)
{

}

Exception::~Exception (void)
{

}

SyntaxError::SyntaxError (const char *what_msg) noexcept : Exception (what_msg)
{

}

SyntaxError::~SyntaxError (void)
{

}

size_t
SyntaxError::get_column (void) const noexcept
{
  return column;
}

size_t
SyntaxError::get_line (void) const noexcept
{
  return line;
}

void
SyntaxError::set_column (size_t line_) noexcept
{
  column = line_;
}

void
SyntaxError::set_line (size_t line_) noexcept
{
  line = line_;
}

UnexpectedToken::UnexpectedToken (void) noexcept : SyntaxError("unexpected token")
{
  expected = "(null)";
  unexpected = "(null)";
}

UnexpectedToken::~UnexpectedToken (void)
{

}

const char *
UnexpectedToken::get_expected (void) const noexcept
{
  return expected;
}

const char *
UnexpectedToken::get_unexpected (void) const noexcept
{
  return unexpected;
}

void
UnexpectedToken::set_expected (const char *token_type) noexcept
{
  if (token_type != nullptr)
    expected = token_type;
}

void
UnexpectedToken::set_unexpected (const char *token_type) noexcept
{
  if (token_type != nullptr)
    unexpected = token_type;
}

UnknownComponent::UnknownComponent (void) noexcept : SyntaxError("unknown component")
{

}

UnknownComponent::~UnknownComponent (void)
{

}

} /* namespace AmpC */

} /* namespace Gnuitar */

