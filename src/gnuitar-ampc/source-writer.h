#ifndef GNUITAR_SOURCE_WRITER_H
#define GNUITAR_SOURCE_WRITER_H

#include <gnuitar-ampc/circuit.h>
#include <gnuitar-ampc/visitor.h>

#include <ostream>

namespace Gnuitar
{

namespace AmpC
{

class SourceWriter final : public Visitor
{
  /** headers to include with brackets */
  std::vector<std::string> bracket_includes;
  /** members of the amplifier structure */
  std::vector<std::string> members;
  /** statements in the initialization function */
  std::vector<std::string> init_stmts;
  /** variables declared the process function */
  std::vector<std::string> process_vars;
  /** statements in the process function */
  std::vector<std::string> process_stmts;
  /** the number of capacitors in the amplifier */
  size_t capacitor_count;
  /** the number of resistors in the amplifier */
  size_t resistor_count;
public:
  SourceWriter (void) noexcept;
  ~SourceWriter (void);
  void visit (const Resistor& resisitor) noexcept;
  void visit (const Capacitor& capacitor) noexcept;
  void visit (const Voltage& voltage) noexcept;
  void write (std::ostream& out) const noexcept;
protected:
  void add_bracket_include (const std::string& include_path) noexcept;
  void add_member (const std::string& member) noexcept;
  void add_init_stmt (const std::string& init_stmt) noexcept;
  void add_process_var (const std::string& process_var) noexcept;
  void add_process_stmt (const std::string& process_line) noexcept;
  void add_process_stmt (std::string&& process_line) noexcept;
  void write_includes (std::ostream& output) const noexcept;
  void write_amp (std::ostream& output) const noexcept;
  void write_init_function (std::ostream& output) const noexcept;
  void write_process_function (std::ostream& output) const noexcept;
}; /* class SourceWriter */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_SOURCE_WRITER_H */

