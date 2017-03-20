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
  bool eof (void) const;
  bool read (Circuit& circuit);
protected:
  bool read_single (Circuit& circuit);
  void read (Resistor& resistor);
  void read (Capacitor& capacitor);
  void read (Voltage& voltage);
  bool read (Component& component);
  void toss_unexpected_token (const char *expected, const char *unexpected) const;
  void toss_unknown_component (void) const;
private:
  bool read (SpiceToken& token);
  bool peek (SpiceToken& token);
}; /* class SpiceParser */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_SPICE_PARSER_H */

