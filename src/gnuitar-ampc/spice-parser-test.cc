#include <gnuitar-ampc/spice-parser.h>

#include <sstream>

#include <cassert>

int
main (void)
{
  std::stringstream source;
  source << "Rswamp 1 3 200" << std::endl;
  source << "C1 3 4 1.00" << std::endl;
  source << "Rload 4 2 200" << std::endl;
  source << "Cbypass 2 1 1.00" << std::endl;

  Gnuitar::AmpC::SpiceLexer lexer (source);

  Gnuitar::AmpC::SpiceParser parser (lexer);

  Gnuitar::AmpC::Resistor rswamp;
  parser.read (rswamp);
  assert (rswamp.get_label () == "swamp");
  assert (rswamp.get_input () == 1);
  assert (rswamp.get_output () == 3);
  assert (static_cast<int> (rswamp.get_resistance ()) == 200);

  Gnuitar::AmpC::Capacitor c1;
  parser.read (c1);
  assert (c1.get_label () == "1");
  assert (c1.get_input () == 3);
  assert (c1.get_output () == 4);
  assert (static_cast<int> (c1.get_capacitance ()) == 1);

  Gnuitar::AmpC::Circuit circuit;
  parser.read (circuit);

  return 0;
}

