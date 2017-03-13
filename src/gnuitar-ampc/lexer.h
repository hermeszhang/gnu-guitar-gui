#ifndef GNUITAR_AMPC_LEXER_H
#define GNUITAR_AMPC_LEXER_H

#include <cstdlib>
#include <istream>

namespace Gnuitar
{

namespace AmpC
{

class Lexer
{
  std::istream& input;
  size_t line;
  size_t column;
public:
  Lexer (std::istream& input_) noexcept;
  virtual ~Lexer (void);
  bool eof (void) const;
  size_t get_line (void) const noexcept;
  size_t get_column (void) const noexcept;
protected:
  int get (void);
  int peek (void);
}; /* class Lexer */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_LEXER_H */

