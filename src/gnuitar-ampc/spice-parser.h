#ifndef GNUITAR_AMPC_SPICE_PARSER_H
#define GNUITAR_AMPC_SPICE_PARSER_H

#include <gnuitar-ampc/circuit.h>
#include <gnuitar-ampc/spice-lexer.h>

#include <vector>

namespace Gnuitar
{

namespace AmpC
{

class SpiceParser final
{
  SpiceLexer& lexer;
public:
  SpiceParser (SpiceLexer& lexer_) noexcept;
  ~SpiceParser (void);
  bool read (Circuit& circuit);
  bool read (Resistor& resistor);
  bool read (Capacitor& capacitor);
protected:
  void toss_unexpected_token (const char *expected, const char *unexpected) const;
}; /* class SpiceParser */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_SPICE_PARSER_H */

