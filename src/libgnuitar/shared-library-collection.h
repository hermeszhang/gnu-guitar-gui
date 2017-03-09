#ifndef GNUITAR_SHARED_LIBRARY_COLLECTION_H
#define GNUITAR_SHARED_LIBRARY_COLLECTION_H

#include "shared-library.h"

#include <map>
#include <vector>

namespace Gnuitar
{

class SharedLibraryCollection
{
public:
  typedef std::map<std::string, SharedLibrary>::const_iterator const_iterator;
private:
  /** a mapping of the library's path to it's data structure  */
  std::map<std::string, SharedLibrary> shared_libraries;
  /** a list of paths to find libraries at */
  std::vector<std::string> search_paths;
public:
  /* don't want this copied */
  SharedLibraryCollection (const SharedLibraryCollection& shared_library_collection) = delete;
  /* don't want it moved */
  SharedLibraryCollection (SharedLibraryCollection&& shared_library_collection) = delete;
  /** Initializes a shared library collection.
   * This function does not open any libraries.
   */
  SharedLibraryCollection (void) noexcept;
  ~SharedLibraryCollection (void);
  /** adds a path to the list of paths to find libraries at.
   * @param search_path the path to add to the search list
   */
  void add_search_path (const std::string& search_path) noexcept;
  /** adds a comma (or colon) separated list of paths to the search paths
   * to find libraries at.
   * @param search_path_list a comma (or colon) separated list of paths
   */
  void add_search_path_list (const std::string& search_path_list) noexcept;
  /** Updates the list of libraries found at the search paths. */
  void update (void) noexcept;
  /** Returns an iterator to the beginning of the collection */
  const_iterator cbegin (void) const noexcept;
  /** Returns an iterator to the end of the collection */
  const_iterator cend (void) const noexcept;
  /** Locates a shared library by it's path.
   * @returns If the library isn't found, then @ref cend is returned */
  const_iterator find (const std::string& path) const noexcept;
protected:
  /** Opens a shared library by a path.
   * @param path a path, pointing to the location of the library.
   * @returns true if the library was opened successfully, false otherwise.
   */
  bool open (const std::string& path) noexcept;
}; /* class SharedLibraryCollection */

} /* namespace Gnuitar */

#endif /* GNUITAR_SHARED_LIBRARY_COLLECTION_H */

