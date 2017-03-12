#include "source-writer.h"

namespace Gnuitar
{

namespace AmpC
{

SourceWriter::SourceWriter (void) noexcept
{
  capacitor_count = 0;
  resistor_count = 0;

  add_bracket_include ("math.h");
  add_bracket_include ("stdlib.h");

  add_init_stmt ("amp = malloc(sizeof(*amp));");
  add_init_stmt ("if (amp == NULL)");
  add_init_stmt ("  return NULL;");

  add_member ("rate");

  add_process_var ("voltage");
  add_process_var ("resistance");

  add_process_stmt ("/* represent input as a signal */");
  add_process_stmt ("voltage = in;");
  add_process_stmt ("resistance = 0.001f;");
}

SourceWriter::~SourceWriter (void)
{

}

void
SourceWriter::visit (const Resistor& resistor) noexcept
{
  std::string line;

  /** mention which resistor this expression is
   * originating from */
  line = "/* from R";
  line += resistor.get_label ();
  line += " */";
  add_process_stmt(std::move(line));

  line = "resistance += ";
  line += std::to_string(resistor.get_resistance());
  line += ";";
  add_process_stmt(std::move(line));

  resistor_count++;
}

void
SourceWriter::visit (const Capacitor& capacitor) noexcept
{
  auto capacitance = std::to_string (capacitor.get_capacitance ());

  (void) capacitor;
  /* declare the charge for the capacitor */
  std::string q_name;
  q_name += 'c';
  q_name += capacitor.get_label ();
  q_name += "_Q";
  add_member (q_name);

  /* in the function, q is a member of the amp structure */
  q_name = "amp->" + q_name;

  /* mention where the expression is coming from */
  std::string comment;
  comment = "/* from C";
  comment += capacitor.get_label ();
  comment += " */";
  add_process_stmt (std::move (comment));

  std::string q_stmt;
  q_stmt = q_name;
  q_stmt += " = voltage * ";
  q_stmt += capacitance;
  q_stmt += " * (1 - expf(-1.0 / (amp->rate * resistance * " + capacitance + ")));";
  add_process_stmt (std::move (q_stmt));
/*
  q_stmt = "current *= (voltage - (" + q_name + " / " + capacitance + ")) / voltage;";
  add_process_stmt (std::move (q_stmt));
*/
  q_stmt = "voltage -= " + q_name + " / " + capacitance + ";";
  add_process_stmt (std::move (q_stmt));

  /* we now have one more capacitor in the amp */
  capacitor_count++;
}

void
SourceWriter::write (std::ostream& output) const noexcept
{
  write_includes (output);
  write_amp (output);
  write_init_function (output);
  write_process_function (output);
}

void
SourceWriter::add_bracket_include (const std::string& include_path) noexcept
{
  bracket_includes.push_back (include_path);
}

void
SourceWriter::add_member (const std::string& member) noexcept
{
  members.push_back (member);

  std::string init_stmt;
  init_stmt = "amp->";
  init_stmt += member;
  init_stmt += " = 0.0f;";
  add_init_stmt (init_stmt);
}

void
SourceWriter::add_init_stmt (const std::string& init_stmt) noexcept
{
  init_stmts.push_back(init_stmt);
}

void
SourceWriter::add_process_stmt (const std::string& process_stmt) noexcept
{
  process_stmts.push_back(process_stmt);
}

void
SourceWriter::add_process_stmt (std::string&& process_stmt) noexcept
{
  process_stmts.emplace_back(process_stmt);
}

void
SourceWriter::add_process_var (const std::string& process_var) noexcept
{
  process_vars.push_back (process_var);
}

void
SourceWriter::write_includes (std::ostream& output) const noexcept
{
  for (const auto& include_path : bracket_includes)
    output << "#include <" << include_path << ">" << std::endl;

  output << std::endl;
}

void
SourceWriter::write_amp (std::ostream& output) const noexcept
{
  output << "struct GnuitarAmp" << std::endl;
  output << "{" << std::endl;

  for (const auto& member : members)
    output << "  float " << member << ';' << std::endl;

  output << "};" << std::endl;
  output << std::endl;
}

void
SourceWriter::write_init_function (std::ostream& output) const noexcept
{
  output << "struct GnuitarAmp *" << std::endl;
  output << "gnuitar_amp_init (void)" << std::endl;
  output << "{" << std::endl;

  output << "  struct GnuitarAmp *amp;" << std::endl;

  for (const auto& init_stmt : init_stmts)
    output << "  " << init_stmt << std::endl;

  output << "  return amp;" << std::endl;

  output << "}" << std::endl;
  output << std::endl;
}

void
SourceWriter::write_process_function (std::ostream& output) const noexcept
{
  output << "float" << std::endl;
  output << "gnuitar_amp_process_signal (struct GnuitarAmp *amp, float in)" << std::endl;
  output << "{" << std::endl;

  for (const auto& process_var : process_vars)
    output << "  float " << process_var << ";" << std::endl;

  for (const auto& process_stmt : process_stmts)
    output << "  " << process_stmt << std::endl;

  /* return the output */
  output << "  return voltage;" << std::endl;

  output << "}" << std::endl;
  output << std::endl;
}

} /* namespace AmpC */

} /* namespace Gnuitar */

