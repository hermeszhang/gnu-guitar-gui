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
SpiceParser::eof (void) const
{
  return lexer.eof ();
}

bool
SpiceParser::read (Circuit& circuit)
{
  while (!eof ())
    {
      if (!read_single (circuit))
        break;
    }
  return false;
}

bool
SpiceParser::read_single (Circuit& circuit)
{
  Component component;

  if (!read (component))
    return false;

  if (component.get_designator () == 'C')
    {
      Capacitor c (std::move (component));
      read (c);
      circuit.add (std::move (c));
    }
  else if (component.get_designator () == 'R')
    {
      Resistor r (std::move (component));
      read (r);
      circuit.add (std::move (r));
    }
  else if (component.get_designator () == 'V')
    {
      Voltage v (std::move (component));
      read (v);
      circuit.add (std::move (v));
    }
  else
    throw UnknownDesignator (component.get_designator ());

  return true;
}

void
SpiceParser::read (Capacitor& capacitor)
{
  SpiceToken token;

  float capacitance;
  if (!read (token) || !token.get_real_number (&capacitance))
    toss_unexpected_token ("capacitance", nullptr);

  capacitor.set_capacitance (capacitance);
}

void
SpiceParser::read (Resistor& resistor)
{
  SpiceToken token;

  float resistance;
  if (!read (token) || !token.get_real_number (&resistance))
    toss_unexpected_token ("resistance", nullptr);
  else
    resistor.set_resistance (resistance);
}

void
SpiceParser::read (Voltage& voltage)
{
  if (voltage.get_label () == "in")
    return;

  SpiceToken token;
  float voltage_value;
  if (!read (token) || !token.get_real_number (&voltage_value))
    toss_unexpected_token ("voltage", nullptr);
  else
    voltage.set_voltage (voltage_value);
}

bool
SpiceParser::read (Component& component)
{
  SpiceToken token;

  if (!peek (token))
    return false;

  if (token.get_type () != SpiceToken::Type::identifier)
    return false;

  const std::string& data = token.get_data ();
  if (data.size () == 0)
    return false;
  else
    {
      component.set_designator (data[0]);
      component.set_label (data.substr (1));
    }

  lexer.clear_peek_token ();

  long in;
  if (!read (token) || !token.get_integer (&in))
    toss_unexpected_token ("input connection", nullptr);

  long out;
  if (!read (token) || !token.get_integer (&out))
    toss_unexpected_token ("output connection", nullptr);

  component.set_input (in);
  component.set_output (out);

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

bool
SpiceParser::read (SpiceToken& token)
{
  return lexer.read_token (token);
}

bool
SpiceParser::peek (SpiceToken& token)
{
  return lexer.peek_token (token);
}

} /* namespace AmpC */

} /* namespace Gnuitar */

