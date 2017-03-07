#include <libgnuitar/plugin-manager.h>

#include <cerrno>

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <glob.h>
#endif /* _WIN32 */

namespace Gnuitar
{

PluginManager::PluginManager (void) noexcept
{
#ifdef __unix__
/*
  add_ladspa_path("/usr/lib/ladspa");
  add_ladspa_path("/usr/local/lib/ladspa");
*/
#endif /* __unix__ */
}

PluginManager::~PluginManager (void)
{

}

int
PluginManager::open_by_name (const std::string& name) noexcept
{
  std::string filename;
#ifdef _WIN32
  filename = name + ".dll";
#else /* _WIN32 */
  filename = name + ".so";
#endif /* _WIN32 */

  for (const std::string& ladspa_path : ladspa_paths)
    {
      if (open_by_path(ladspa_path + "/" + filename) == 0)
        return 0;
    }

  return ENOENT;
}

int
PluginManager::open_by_path (const std::string& path) noexcept
{
  Plugin plugin (path);
  if (plugin.good ())
    {
      plugins.push_back(plugin);
      return 0;
    }
  return EINVAL;
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

  ladspa_paths.push_back(path);

  return 0;
}

int
PluginManager::find_all_ladspa_plugins (void) noexcept
{
  for (const std::string& ladspa_path : ladspa_paths)
    {
      find_all_ladspa_plugins(ladspa_path);
    }
  return 0;
}

int
PluginManager::find_all_ladspa_plugins (const std::string& path) noexcept
{
#ifdef _WIN32
  (void) path;
  return EFAULT;
#else /* _WIN32 */

  std::string pattern;
  pattern = path + "/*.so";

  glob_t glbuf;

  if (glob(pattern.c_str(), 0, nullptr, &glbuf) != 0)
    return EINVAL;

  for (decltype(glbuf.gl_pathc) i = 0; i < glbuf.gl_pathc; i++)
    {
      open_by_path(glbuf.gl_pathv[i]);
    }

  globfree(&glbuf);

  return 0;
#endif /* _WIN32 */
}

Effect *
PluginManager::get_ladspa_effect (const std::string& name) noexcept
{
  for (const Plugin& plugin : plugins)
    {
      for (size_t i = 0; i < SIZE_MAX; i++)
        {
          auto effect = plugin.get_effect(i);
          if (effect == nullptr)
            break;
          auto effect_name = effect->get_name();
          if (effect_name != nullptr)
            {
              if (effect_name == name)
                return effect;
            }
          delete effect;
        }
    }
  return nullptr;
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

