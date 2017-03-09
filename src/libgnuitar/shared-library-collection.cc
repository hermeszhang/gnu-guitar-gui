#include <libgnuitar/shared-library-collection.h>

#include <algorithm>

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

} /* namespace Gnuitar */

