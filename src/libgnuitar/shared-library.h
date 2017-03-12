#ifndef GNUITAR_LIBRARY_H
#define GNUITAR_LIBRARY_H

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <dlfcn.h>
#endif /* _WIN32 */

#include <string>

namespace Gnuitar
{

class SharedLibrary final
{
#ifdef _WIN32
  HANDLE handle;
#else /* _WIN32 */
  void *handle;
#endif /* _WIN32 */
public:
  SharedLibrary (const SharedLibrary& plugin) = delete;
  SharedLibrary (SharedLibrary&& plugin) noexcept;
  SharedLibrary (void) noexcept;
  SharedLibrary (const std::string& path) noexcept;
  ~SharedLibrary (void);
  bool good (void) const noexcept;
  bool open (const std::string& path) noexcept;
  void *get_symbol (const std::string& symbol_name) const noexcept;
}; /* class SharedLibrary */

} /* namespace Gnuitar */

#endif /* GNUITAR_LIBRARY_H */

