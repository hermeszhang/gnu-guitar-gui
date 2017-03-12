#include <libgnuitar/shared-library-collection.h>

#include <algorithm>

#ifndef _WIN32
#include <glob.h>
#endif /* _WIN32 */

namespace Gnuitar
{

SharedLibraryCollection::SharedLibraryCollection (void) noexcept
{

}

SharedLibraryCollection::~SharedLibraryCollection (void)
{

}

void
SharedLibraryCollection::add_search_path (const std::string& search_path) noexcept
{
  /* check to make sure the path is not empty */
  if (search_path.size () == 0)
    return;
  /* check to see if library already exists */
  auto tmp = std::find (search_paths.begin (), search_paths.end (), search_path);
  if (tmp != search_paths.cend ())
    return;
  /* add the search path */
  search_paths.push_back (search_path);
}

void
SharedLibraryCollection::add_search_path_list (const std::string& search_path_list) noexcept
{
  std::string search_path;
  for (auto c : search_path_list)
    {
      if ((c == ';') || (c == ':'))
        {
          add_search_path (search_path);
          search_path.clear ();
        }
      else
       search_path.push_back (c);
    }
  /* in case the path list does not end
   * in a semicolon or colon */
  add_search_path(search_path);
}

void
SharedLibraryCollection::add_search_path_list_from_env (const std::string& search_path_list_key) noexcept
{
  std::string search_path_list = std::getenv(search_path_list_key.c_str());
  add_search_path_list(search_path_list);
}

void
SharedLibraryCollection::add_shared_library (const std::string& path, SharedLibrary&& shared_library) noexcept
{
  /* check to make sure the library isn't already added */
  auto it = shared_libraries.find (path);
  if (it != shared_libraries.end ())
    return;
  shared_libraries.emplace(path, std::move (shared_library));
}

bool
SharedLibraryCollection::open_at_path (const std::string& path) noexcept
{
  SharedLibrary shared_library (path);
  if (shared_library.good ())
    {
      add_shared_library(path, std::move(shared_library));
      return true;
    }
  return false;
}

void
SharedLibraryCollection::update (void) noexcept
{
  for (const std::string& search_path : search_paths)
    update_at_path (search_path);
}

void
SharedLibraryCollection::update_at_path (const std::string& path) noexcept
{
#ifndef _WIN32
  std::string glob_pattern (path + "/*.so");
  glob_t gl;
  if (glob (glob_pattern.c_str (), 0, NULL, &gl) != 0)
    return;
  for (decltype(gl.gl_pathc) i = 0; i < gl.gl_pathc; i++)
    open_at_path(gl.gl_pathv[i]);
  globfree (&gl);
#else /* _WIN32 */
  // TODO windows implementation
  (void) path;
#endif /* _WIN32 */
}

SharedLibraryCollection::const_iterator
SharedLibraryCollection::cbegin (void) const noexcept
{
  return shared_libraries.cbegin ();
}

SharedLibraryCollection::const_iterator
SharedLibraryCollection::cend (void) const noexcept
{
  return shared_libraries.cend ();
}

SharedLibraryCollection::const_iterator
SharedLibraryCollection::find (const std::string& path) const noexcept
{
  return shared_libraries.find (path);
}

} /* namespace Gnuitar */

