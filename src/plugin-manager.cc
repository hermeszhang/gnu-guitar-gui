#include <gnuitar/plugin-manager.h>

#include <cerrno>

namespace Gnuitar
{

PluginManager::PluginManager (void) noexcept
{

}

PluginManager::~PluginManager (void)
{

}

int
PluginManager::open (const char *path) noexcept
{
  Plugin plugin (path);
  if (plugin.good ())
    plugins.push_back(plugin);
  else
    return EINVAL;
  return 0;
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

