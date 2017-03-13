#ifndef GNUITAR_AMPC_SPICE_LEXER_H
#define GNUITAR_AMPC_SPICE_LEXER_H

#include <gnuitar-ampc/lexer.h>
#include <gnuitar-ampc/spice-token.h>

namespace Gnuitar
{

namespace AmpC
{

class SpiceLexer final : public Lexer
{
  SpiceToken next_token;
  bool next_token_available;
public:
  SpiceLexer (std::istream& input_) noexcept;
  ~SpiceLexer (void);
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
protected:
  bool read_identifier (SpiceToken& token);
  bool read_number (SpiceToken& token);
  bool read_symbol (SpiceToken& token);
  /** goes to the next character indicating a token */
  bool next (void);
}; /* class SpiceLexer */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_SPICE_LEXER_H */

