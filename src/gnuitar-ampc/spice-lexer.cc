#include "spice-lexer.h"

#include "exception.h"

namespace {

inline bool is_whitespace (char c)
{
  if ((c == ' ')
   || (c == '\t')
   || (c == '\n')
   || (c == '\r'))
    return true;
  return false;
}

inline bool is_alpha (char c)
{
  if (((c >= 'a') && (c <= 'z'))
   || ((c >= 'A') && (c <= 'Z'))
   || (c == '_'))
    return true;
  return false;
}

inline bool is_numerical (char c)
{
  if ((c >= '0') && (c <= '9'))
    return true;
  return false;
}

inline bool is_identifier (char c)
{
  if (is_alpha (c) || is_numerical (c))
    return true;
  return false;
}

inline bool is_symbol (char c)
{
  if (c == ';')
    return true;
  return false;
}

} /* namespace */

namespace Gnuitar
{

namespace AmpC
{

SpiceLexer::SpiceLexer (std::istream& input_) noexcept : input(input_)
{
  next_token_available = false;
  line = 1;
  column = 1;
}

SpiceLexer::~SpiceLexer (void)
{

}

bool
SpiceLexer::eof (void) const noexcept
{
  return next_token_available || input.eof ();
}

bool
SpiceLexer::peek_token (SpiceToken& token)
{
  if (!next_token_available)
    {
      if (!read_token(next_token))
        return false;
    }

  token = next_token;

  return true;
}

bool
SpiceLexer::read_token (SpiceToken& token)
{
  if (next_token_available)
    {
      token = std::move (next_token);
      next_token_available = false;
      return true;
    }

  /* skip to the next character
   * that may indicate a token */
  next ();

  token.clear ();

  auto c = peek ();
  if (c == EOF)
    return false;

  if (!read_identifier (token)
   && !read_number (token)
   && !read_symbol (token))
    throw Exception ("failed to read token");

  return true;
}

void
SpiceLexer::clear_peek_token (void)
{
  next_token_available = false;
}

size_t
SpiceLexer::get_line (void) const noexcept
{
  return line;
}

size_t
SpiceLexer::get_column (void) const noexcept
{
  return column;
}

bool
SpiceLexer::read_identifier (SpiceToken& token)
{
  auto c = peek ();
  if (!is_alpha (c))
    return false;

  for (;;)
    {
      c = get ();
      if (c == EOF)
        break;
      else if (!is_identifier (c))
        break;
      token.add_char(c);
    }

  token.set_type (SpiceToken::Type::identifier);

  return true;
}

bool
SpiceLexer::read_number (SpiceToken& token)
{
  auto c = peek ();

  if (!is_numerical (c) && (c != '.'))
    return false;

  auto dot_found = false;

  for (;;)
    {
      c = get ();
      if (c == '.')
        dot_found = true;
      else if (!is_numerical (c))
        break;
      token.add_char (c);
    }

  if (dot_found)
    token.set_type (SpiceToken::Type::real_number);
  else
    token.set_type (SpiceToken::Type::integer);

  return true;
}

bool
SpiceLexer::read_symbol (SpiceToken& token)
{
  auto c = peek ();

  if (!is_symbol (c))
    return false;
  while (!is_symbol (c))
    {
      token.add_char(c);
      c = get ();
      if (c == EOF)
        break;
    }

  token.set_type (SpiceToken::Type::symbol);

  return true;
}

bool
SpiceLexer::next (void)
{
  for (;;)
    {
      auto c = peek ();
      if (c == EOF)
        return false;
      else if (!is_whitespace (c))
        break;
      get ();
    }
  return true;
}

int
SpiceLexer::peek (void) noexcept
{
  return input.peek ();
}

int
SpiceLexer::get (void) noexcept
{
  auto c = input.get ();
  if (c == '\n')
    {
      line++;
      column = 1;
    }
  else
    column++;
  return c;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

