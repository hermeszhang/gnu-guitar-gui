#ifndef GNUITAR_PLUGIN_MANAGER_H
#define GNUITAR_PLUGIN_MANAGER_H

#include "plugin.h"

#include <vector>

namespace Gnuitar
{

class PluginManager
{
  std::vector<Plugin> plugins;
public:
  PluginManager (void) noexcept;
  ~PluginManager (void);
  int open(const char *path) noexcept;
  Plugin *plugin (size_t index) noexcept;
  size_t plugin_count(void) const noexcept;
}; /* class PluginManager */

} /* namespace Gnuitar */

#endif /* GNUITAR_PLUGIN_MANAGER_H */

