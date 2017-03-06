#ifndef GNUITAR_PLUGIN_MANAGER_H
#define GNUITAR_PLUGIN_MANAGER_H

#include "plugin.h"

#include <vector>
#include <string>

namespace Gnuitar
{

class PluginManager
{
  std::vector<Plugin> plugins;
  std::vector<std::string> ladspa_paths;
public:
  PluginManager (void) noexcept;
  ~PluginManager (void);
  int open(const std::string& name) noexcept;
  int add_ladspa_path(const std::string& path) noexcept;
  int parse_ladspa_paths(const std::string& paths) noexcept;
  int parse_ladspa_env(void) noexcept;
  Plugin *plugin (size_t index) noexcept;
  size_t plugin_count(void) const noexcept;
}; /* class PluginManager */

} /* namespace Gnuitar */

#endif /* GNUITAR_PLUGIN_MANAGER_H */

