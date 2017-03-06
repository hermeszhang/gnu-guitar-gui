#ifndef GNUITAR_LADSPA_H
#define GNUITAR_LADSPA_H

#include <ladspa.h>

#include <libgnuitar/plugin.h>

namespace Gnuitar
{

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
}; /* class Effect */

class Plugin final : public Gnuitar::Plugin
{
  const LADSPA_Descriptor *descriptor;
public:
  Plugin(void) noexcept;
  Plugin(const std::string& path) noexcept;
  ~Plugin(void);
  Effect *get_effect(size_t index) const;
}; /* class Plugin */

} /* namespace LADSPA */

} /* namespace Gnuitar */

#endif /* GNUITAR_LADSPA_H */

