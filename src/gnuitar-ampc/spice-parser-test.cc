#include <gnuitar-ampc/spice-parser.h>

#include <gnuitar-ampc/visitor.h>

#include <sstream>

#include <cassert>

namespace
{

class CircuitChecker final : public Gnuitar::AmpC::Visitor
{
public:
  ~CircuitChecker (void)
  {

  }
  void
  visit (const Gnuitar::AmpC::Resistor& r) noexcept
  {
    assert (r.get_label () == "swamp");
    assert (r.get_input () == 1);
    assert (r.get_output () == 3);
  }
  void
  visit (const Gnuitar::AmpC::Capacitor& c) noexcept
  {
    assert (c.get_label () == "1");
    assert (c.get_input () == 3);
    assert (c.get_output () == 1);
  }
  void
  visit (const Gnuitar::AmpC::Voltage&) noexcept
  {

  }
};

} /* namespace */

int
main (void)
{
  std::stringstream source;
  source << "Rswamp 1 3 200" << std::endl;
  source << "C1 3 1 1.00" << std::endl;

  Gnuitar::AmpC::SpiceLexer lexer (source);

  Gnuitar::AmpC::SpiceParser parser (lexer);

  Gnuitar::AmpC::Circuit circuit;
  parser.read (circuit);

  CircuitChecker checker;

  circuit.accept (checker);

  return 0;
}

