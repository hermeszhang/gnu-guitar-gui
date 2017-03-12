#ifndef GNUITAR_EXCEPTION_H
#define GNUITAR_EXCEPTION_H

#include <stdexcept>

namespace Gnuitar
{

class Exception : public std::runtime_error
{
public:
  Exception (const char *what_msg) noexcept;
  virtual ~Exception (void);
}; /* class Exception */

class SystemException final : public Exception
{
  int errno_;
public:
  SystemException (const char *what_msg, int errno__) noexcept;
  ~SystemException (void);
  int get_errno (void) const noexcept;
};

} /* namespace Gnuitar */

#endif /* GNUITAR_EXCEPTION_H */

