#ifndef GNUITAR_PLUGIN_H
#define GNUITAR_PLUGIN_H

#include <ladspa.h>
#include <lv2.h>

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <dlfcn.h>
#endif /* _WIN32 */

namespace Gnuitar
{

class Effect final
{
  enum class Type
  {
    ladspa,
    lv2
  } type;
  union
  {
    const LADSPA_Descriptor *ladspa_descriptor;
    const LV2_Descriptor *lv2_descriptor;
  };
  union
  {
    LADSPA_Handle ladspa_handle;
    LV2_Handle lv2_handle;
  };
public:
  Effect (void) = delete;
  Effect (const Effect&) = delete;
  Effect (const LADSPA_Descriptor *ladspa_) noexcept;
  Effect (const LV2_Descriptor *lv2_) noexcept;
  ~Effect (void);
  int activate(size_t sample_rate) noexcept;
  const char *get_maker (void) const noexcept;
  const char *get_name (void) const noexcept;
  int process (float *sample_array, size_t sample_count) noexcept;
protected:
  int connect_to_ladspa_ports(float *sample_array) noexcept;
}; /* class Effect */

class Plugin final
{
#ifdef _WIN32
  HANDLE handle;
#else /* _WIN32 */
  void *handle;
#endif /* _WIN32 */
public:
  Plugin (const char *path) noexcept;
  Plugin (void) noexcept;
  ~Plugin (void);
  bool good (void) const noexcept;
  Effect *get_effect (size_t index) noexcept;
protected:
  int load (const char *path) noexcept;
  int find_entry (void) noexcept;
private:
  enum class Type
  {
    ladspa,
    lv2
  } type;
  union
  {
    LADSPA_Descriptor_Function ladspa_descriptor;
    LV2_Descriptor_Function lv2_descriptor;
  };
}; /* class Plugin */

} /* namespace Gnuitar */

#endif /* GNUITAR_PLUGIN_H */

