#include "exception.h"

namespace Gnuitar
{

Exception::Exception (const char *what_msg) noexcept : std::runtime_error (what_msg)
{

}

Exception::~Exception (void)
{

}

SystemException::SystemException (const char *what_msg, int errno__) noexcept : Exception (what_msg), errno_(errno__)
{

}

SystemException::~SystemException (void)
{

}

int SystemException::get_errno (void) const noexcept
{
  return errno_;
}

} /* namespace Gnuitar */

