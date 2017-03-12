#ifndef GNUITAR_AMPC_SPICE_LEXER_H
#define GNUITAR_AMPC_SPICE_LEXER_H

#include <gnuitar-ampc/spice-token.h>

#include <istream>

namespace Gnuitar
{

namespace AmpC
{

class SpiceLexer final
{
  std::istream& input;
  SpiceToken next_token;
  bool next_token_available;
  size_t line;
  size_t column;
public:
  SpiceLexer (std::istream& input_) noexcept;
  ~SpiceLexer (void);
  bool eof (void) const noexcept;
  /** reads a token.
   * @returns true if a token is read.
   *  false if the end of file was reached */
  bool read_token (SpiceToken& token);
  /** reads a token, without skipping past it.
   * @returns true if a token is read,
   *  false if the end of file was reached */
  bool peek_token (SpiceToken& token);
  /** erases the peek token  */
  void clear_peek_token (void);
  size_t get_line (void) const noexcept;
  size_t get_column (void) const noexcept;
protected:
  bool read_identifier (SpiceToken& token);
  bool read_number (SpiceToken& token);
  bool read_symbol (SpiceToken& token);
  /** goes to the next character indicating a token */
  bool next (void);
  int peek (void) noexcept;
  int get (void) noexcept;
}; /* class SpiceLexer */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_SPICE_LEXER_H */

