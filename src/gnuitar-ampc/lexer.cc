#include <gnuitar-ampc/lexer.h>

namespace Gnuitar
{

namespace AmpC
{

Lexer::Lexer (std::istream& input_) noexcept : input(input_)
{
  line = 1;
  column = 1;
}

Lexer::~Lexer (void)
{

}

bool
Lexer::eof (void) const
{
  return input.eof ();
}

size_t
Lexer::get_line (void) const noexcept
{
  return line;
}

size_t
Lexer::get_column (void) const noexcept
{
  return line;
}

int
Lexer::get (void)
{
  auto c = input.get ();

  if (c == '\n')
    {
      line++;
      column = 1;
    }
  else if (c != EOF)
    column++;

  return c;
}

int
Lexer::peek (void)
{
  return input.peek ();
}

} /* namespace AmpC */

} /* namespace Gnuitar */

