#include <libgnuitar/shared-library.h>

#ifdef GNUITAR_DEBUG
#include <iostream>
#endif /* GNUITAR_DEBUG */

namespace Gnuitar {

SharedLibrary::SharedLibrary (SharedLibrary&& shared_library) noexcept
{
  handle = std::move(shared_library.handle);
  shared_library.handle = nullptr;
}

SharedLibrary::SharedLibrary (void) noexcept
{
  handle = nullptr;
}

SharedLibrary::SharedLibrary (const std::string& path) noexcept : SharedLibrary()
{
  open (path);
}

SharedLibrary::~SharedLibrary (void)
{
  if (handle != nullptr)
    {
#ifdef _WIN32
#else /* _WIN32 */
      dlclose (handle);
#endif /* _WIN32 */
    }
}

bool
SharedLibrary::good (void) const noexcept
{
  return handle != nullptr;
}

bool
SharedLibrary::open (const std::string& path) noexcept
{
#ifdef _WIN32
  (void) path;
  return false;
#else /* _WIN32 */
  /* open a new handle, in a
   * temporary variable (in case it fails) */
  auto tmp_handle = dlopen (path.c_str(), RTLD_LAZY);
  if (tmp_handle == nullptr)
    {
#ifdef GNUITAR_DEBUG
      std::cerr << "gnuitar: failed to load " << path << std::endl;
      std::cerr << "dlerror: " << dlerror() << std::endl;
#endif /* GNUITAR_DEBUG */
      return false;
    }
  /* close the previous handle,
   * if it was opened */
  if (handle != nullptr)
    dlclose(handle);
  /* set new handle */
  handle = tmp_handle;
  return true;
#endif /* _WIN32 */
}

void *
SharedLibrary::get_symbol (const std::string& symbol_name) noexcept
{
#ifdef _WIN32
  return nullptr;
#else /* _WIN32 */
  if (handle == nullptr)
    return nullptr;
  return dlsym(handle, symbol_name.c_str());
#endif /* _WIN32 */
}

} /* namespace Gnuitar */

