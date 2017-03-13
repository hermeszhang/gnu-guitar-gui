#include <gnuitar-ampc/spice-lexer.h>

#include <cassert>
#include <sstream>

int
main (void)
{
  std::stringstream data;
  data << "id2 223 2.4";

  Gnuitar::AmpC::SpiceLexer spice_lexer (data);

  Gnuitar::AmpC::SpiceToken token;
  spice_lexer.read_token (token);
  assert (token.get_data () == "id2");
  assert (token.get_type () == Gnuitar::AmpC::SpiceToken::Type::identifier);

  spice_lexer.read_token (token);
  assert (token.get_data () == "223");
  assert (token.get_type () == Gnuitar::AmpC::SpiceToken::Type::integer);

  spice_lexer.read_token (token);
  assert (token.get_data () == "2.4");
  assert (token.get_type () == Gnuitar::AmpC::SpiceToken::Type::real_number);

  return 0;
}

