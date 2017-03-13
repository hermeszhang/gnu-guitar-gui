#ifndef GNUITAR_AMPC_EXCEPTION_H
#define GNUITAR_AMPC_EXCEPTION_H

#include <stdexcept>

/* for size_t */
#include <cstdlib>

namespace Gnuitar
{

namespace AmpC
{

class Exception : public std::runtime_error
{
public:
  Exception (const char *what_msg) noexcept;
  ~Exception (void);
}; /* class Exception */

class SyntaxError : public Exception
{
  size_t line;
  size_t column;
public:
  SyntaxError (const char *what_msg) noexcept;
  ~SyntaxError (void);
  void set_line (size_t line_) noexcept;
  void set_column (size_t column_) noexcept;
  size_t get_line (void) const noexcept;
  size_t get_column (void) const noexcept;
}; /* class SyntaxError */

class UnexpectedToken final : public SyntaxError
{
  const char *expected;
  const char *unexpected;
public:
  UnexpectedToken (void) noexcept;
  ~UnexpectedToken (void);
  void set_expected (const char *token_type) noexcept;
  void set_unexpected (const char *token_type) noexcept;
  const char *get_expected (void) const noexcept;
  const char *get_unexpected (void) const noexcept;
}; /* class BadToken */

class UnknownComponent final : public SyntaxError
{
public:
  UnknownComponent (void) noexcept;
  ~UnknownComponent (void);
}; /* class UnknownComponent */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_EXCEPTION_H */

