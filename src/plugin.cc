#include "plugin.h"

#include <cerrno>

#ifdef GNUITAR_DEBUG
#include <iostream>
#endif /* GNUITAR_DEBUG */

namespace Gnuitar {

Effect::Effect (const LADSPA_Descriptor *ladspa_descriptor_) noexcept : ladspa_descriptor(ladspa_descriptor_)
{
  type = Effect::Type::ladspa;
}

Effect::Effect (const LV2_Descriptor *lv2_descriptor_) noexcept : lv2_descriptor(lv2_descriptor_)
{
  type = Effect::Type::lv2;
}

Effect::~Effect (void)
{

}

const char *
Effect::get_maker (void) const noexcept
{
  switch (type)
  {
    case Effect::Type::ladspa:
    if (ladspa_descriptor == nullptr)
      return nullptr;
    return ladspa_descriptor->Maker;

    case Effect::Type::lv2:
    return nullptr;
  }

  return nullptr;
}

Plugin::Plugin (void) noexcept
{
  is_open = false;
}

Plugin::Plugin (const char *path) noexcept
{
  load (path);
}

Plugin::~Plugin (void)
{
  if (is_open)
    {
#ifdef _WIN32
#else /* _WIN32 */
      dlclose (handle);
#endif /* _WIN32 */
    }
}

Effect *
Plugin::get_effect (size_t index) noexcept
{
  if (!is_open)
    return nullptr;

  union
  {
    const LADSPA_Descriptor *ladspa_effect;
    const LV2_Descriptor *lv2_effect;
  };

  switch (type)
  {
    case Plugin::Type::ladspa:
    ladspa_effect = ladspa_descriptor(index);
    if (ladspa_effect == nullptr)
      return nullptr;
    return new Effect(ladspa_effect);

    case Plugin::Type::lv2:
    lv2_effect = lv2_descriptor(index);
    if (lv2_effect == nullptr)
      return nullptr;
    return new Effect(lv2_effect);
  }

  return nullptr;
}

int
Plugin::load (const char *path) noexcept
{
  is_open = false;
#ifdef _WIN32
  (void) path;
  return EINVAL;
#else /* _WIN32 */
  handle = dlopen (path, RTLD_LAZY);
  if (handle == nullptr)
    {
#ifdef GNUITAR_DEBUG
      std::cerr << "gnuitar: failed to load " << path << std::endl;
      std::cerr << "dlerror: " << dlerror() << std::endl;
#endif /* GNUITAR_DEBUG */
      return ENOENT;
    }
  if (find_entry () != 0)
    {
      dlclose(handle);
      return EINVAL;
    }
  is_open = true;
  return 0;
#endif /* _WIN32 */
}

int
Plugin::find_entry (void) noexcept
{
#ifdef _WIN32
  return EINVAL;
#else /* _WIN32 */
  if (handle == nullptr)
    return EFAULT;
  ladspa_descriptor = (LADSPA_Descriptor_Function)(dlsym(handle, "ladspa_descriptor"));
  if (ladspa_descriptor != nullptr)
    {
      type = Plugin::Type::ladspa;
      return 0;
    }
  lv2_descriptor = (LV2_Descriptor_Function)(dlsym(handle, "lv2_descriptor"));
  if (lv2_descriptor != nullptr)
    {
      type = Plugin::Type::lv2;
      return 0;
    }
  return EINVAL;
#endif /* _WIN32 */
  return 0;
}

} /* namespace Gnuitar */

