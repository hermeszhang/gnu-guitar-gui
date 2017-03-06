#include <gnuitar/plugin-manager.h>

#include <cerrno>

#include <iostream>

namespace Gnuitar
{

PluginManager::PluginManager (void) noexcept
{
#ifdef __unix__
  add_ladspa_path("/usr/lib/ladspa");
  add_ladspa_path("/usr/local/lib/ladspa");
#endif /* __unix__ */
}

PluginManager::~PluginManager (void)
{

}

int
PluginManager::open (const std::string& name) noexcept
{
  std::string filename;
#ifdef _WIN32
  filename = name + ".dll";
#else /* _WIN32 */
  filename = name + ".so";
#endif /* _WIN32 */

  for (const std::string& ladspa_path : ladspa_paths)
    {
      std::string plugin_path(ladspa_path + "/" + filename);
      Plugin plugin (plugin_path);
      if (plugin.good ())
        {
          plugins.push_back(plugin);
          return 0;
        }
    }

  return ENOENT;
}

int
PluginManager::add_ladspa_path (const std::string& path) noexcept
{
  if (path.size() == 0)
    return EINVAL;

  /* check if path already exists */
  for (const std::string& path_entry : ladspa_paths)
    {
      if (path_entry == path)
        return 0;
    }

  std::cout << "gnuitar: adding ladspa search path: " << path << std::endl;
  ladspa_paths.push_back(path);

  return 0;
}

int
PluginManager::parse_ladspa_paths (const std::string& paths) noexcept
{
  std::string tmp_path;
  for (auto c : paths)
    {
      if (c == ':')
        {
          add_ladspa_path(tmp_path);
          tmp_path.clear();
        }
      else
        {
          tmp_path.push_back(c);
        }
    }

  if (tmp_path.size() > 0)
    add_ladspa_path(tmp_path);

  return 0;
}

int
PluginManager::parse_ladspa_env (void) noexcept
{
  std::string ladspa_paths = std::getenv("LADSPA_PATH");
  if (ladspa_paths.size() == 0)
    return 0;
  return parse_ladspa_paths(ladspa_paths);
}

Plugin *
PluginManager::plugin (size_t index) noexcept
{
  if (index >= plugins.size ())
    return nullptr;
  return &plugins[index];
}

size_t
PluginManager::plugin_count (void) const noexcept
{
  return plugins.size();
}

} /* namespace Gnuitar */

