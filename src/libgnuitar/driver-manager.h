#ifndef GNUITAR_DRIVER_MANAGER_H
#define GNUITAR_DRIVER_MANAGER_H

#include <libgnuitar/driver.h>
#include <libgnuitar/effect-list.h>
#include <libgnuitar/shared-library-collection.h>

namespace Gnuitar
{

class DriverManager final
{
  SharedLibraryCollection shared_libs;
  Driver *driver;
public:
  DriverManager (void) noexcept;
  ~DriverManager (void);
  Effect * add_effect (const std::string& name);
  EffectList get_effect_list (void) const;
  void start_driver (void);
  void stop_driver (void);
  void update_libs (void) noexcept;
protected:
  const LADSPA_Descriptor * search_ladspa_descriptor (const std::string& name) noexcept;
}; /* class DriverManager */

} /* namespace Gnuitar */

#endif /* GNUITAR_DRIVER_MANAGER_H */

