#include <gnuitar-ampc/spice-parser.h>
#include <gnuitar-ampc/exception.h>

namespace Gnuitar
{

namespace AmpC
{

SpiceParser::SpiceParser (SpiceLexer& lexer_) noexcept : lexer(lexer_)
{

}

SpiceParser::~SpiceParser (void)
{

}

bool
SpiceParser::read (Circuit& circuit)
{
  while (!lexer.eof ())
    {
      Resistor r;
      if (read (r))
        {
          circuit.add (r);
          continue;
        }
    
      Capacitor c;
      if (read (c))
        {
          circuit.add (c);
          continue;
        }

      toss_unknown_component ();
    }

  return false;
}

bool
SpiceParser::read (Capacitor& capacitor)
{
  SpiceToken token;

  if (!lexer.peek_token (token))
    return false;

  if (token.get_type () != SpiceToken::Type::identifier)
    return false;

  const std::string& data = token.get_data ();
  if (data.size () == 0)
    return false;
  else if (data[0] != 'C')
    return false;

  capacitor.set_label (data.substr (1));

  lexer.clear_peek_token ();

  long in;
  if (!lexer.read_token (token) || !token.get_integer (&in))
    toss_unexpected_token ("input connection", nullptr);

  long out;
  if (!lexer.read_token (token) || !token.get_integer (&out))
    toss_unexpected_token ("output connection", nullptr);

  float capacitance;
  if (!lexer.read_token (token) || !token.get_real_number (&capacitance))
    toss_unexpected_token ("capacitance", nullptr);

  capacitor.set_capacitance (capacitance);
  capacitor.set_input (in);
  capacitor.set_output (out);

  return true;
}

bool
SpiceParser::read (Resistor& resistor)
{
  SpiceToken token;

  if (!lexer.peek_token (token))
    return false;

  if (token.get_type () != SpiceToken::Type::identifier)
    return false;

  const std::string& data = token.get_data ();
  if (data.size () == 0)
    return false;
  else if (data[0] != 'R')
    return false;

  resistor.set_label (data.substr (1));

  lexer.clear_peek_token ();

  long in;
  if (!lexer.read_token (token) || !token.get_integer (&in))
    toss_unexpected_token ("input connection", nullptr);

  long out;
  if (!lexer.read_token (token) || !token.get_integer (&out))
    toss_unexpected_token ("output connection", nullptr);

  float resistance;
  if (!lexer.read_token (token) || !token.get_real_number (&resistance))
    toss_unexpected_token ("resistance", nullptr);

  resistor.set_resistance (resistance);
  resistor.set_input (in);
  resistor.set_output (out);

  return true;
}

void
SpiceParser::toss_unexpected_token (const char *expected, const char *unexpected) const
{
  UnexpectedToken unexpected_token;
  unexpected_token.set_expected (expected);
  unexpected_token.set_unexpected (unexpected);
  unexpected_token.set_line (lexer.get_line ());
  unexpected_token.set_column (lexer.get_column ());
  throw unexpected_token;
}

void
SpiceParser::toss_unknown_component (void) const
{
  UnknownComponent unknown_component;
  unknown_component.set_line (lexer.get_line ());
  unknown_component.set_column (lexer.get_column ());
  throw unknown_component;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

