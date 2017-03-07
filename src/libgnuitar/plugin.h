#ifndef GNUITAR_PLUGIN_H
#define GNUITAR_PLUGIN_H

#include <ladspa.h>
#include <lv2.h>

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <dlfcn.h>
#endif /* _WIN32 */

#include <string>

namespace Gnuitar
{

class Effect
{
public:
  virtual ~Effect (void);
  virtual bool connect (float *sample_array) noexcept = 0;
  virtual void run (size_t sample_count) noexcept = 0;
}; /* class Effect */

class Plugin
{
#ifdef _WIN32
  HANDLE handle;
#else /* _WIN32 */
  void *handle;
#endif /* _WIN32 */
public:
  Plugin (void) noexcept;
  Plugin (const std::string& path) noexcept;
  virtual ~Plugin (void);
  virtual bool good (void) const noexcept;
  virtual bool open (const std::string& path) noexcept;
protected:
  virtual void *get_symbol(const std::string& symbol_name) noexcept;
}; /* class Plugin */

namespace LADSPA
{

class Effect final : public Gnuitar::Effect
{
  const LADSPA_Descriptor *descriptor;
  LADSPA_Handle handle;
public:
  Effect(const LADSPA_Descriptor *descriptor_) noexcept;
  ~Effect(void);
  std::string get_name(void) const noexcept;
  std::string get_maker(void) const noexcept;
  bool instantiate (size_t rate) noexcept;
  bool activate (void) noexcept;
  bool deactivate (void) noexcept;
  bool connect (float *sample_array) noexcept;
  void run (size_t sample_count) noexcept;
}; /* class Effect */

class Plugin final : public Gnuitar::Plugin
{
  LADSPA_Descriptor_Function descriptor_function;
public:
  Plugin (void) noexcept;
  Plugin (const std::string& path) noexcept;
  ~Plugin (void);
  LADSPA::Effect * get_effect (size_t index) const noexcept;
  bool good (void) const noexcept;
  bool open (const std::string& path) noexcept;
}; /* class Plugin */

} /* namespace LADSPA */

} /* namespace Gnuitar */

#endif /* GNUITAR_PLUGIN_H */

