#include <gnuitar-ampc/spice-parser.h>

#include <sstream>

#include <cassert>

int
main (void)
{
  std::stringstream source;
  source << "Rswamp 1 3 200" << std::endl;

  Gnuitar::AmpC::SpiceLexer lexer (source);

  Gnuitar::AmpC::SpiceParser parser (lexer);

  Gnuitar::AmpC::Resistor rswamp;
  parser.read (rswamp);

  assert (rswamp.get_label () == "swamp");
  assert (rswamp.get_input () == 1);
  assert (rswamp.get_output () == 3);
  assert (static_cast<int> (rswamp.get_resistance ()) == 200);

  return 0;
}

