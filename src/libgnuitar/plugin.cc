#include <libgnuitar/plugin.h>

#include <cerrno>

#ifdef GNUITAR_DEBUG
#include <iostream>
#endif /* GNUITAR_DEBUG */

namespace Gnuitar {

Effect::Effect (const LADSPA_Descriptor *ladspa_descriptor_) noexcept : ladspa_descriptor(ladspa_descriptor_)
{
  type = Effect::Type::ladspa;
  ladspa_handle = nullptr;
}

Effect::Effect (const LV2_Descriptor *lv2_descriptor_) noexcept : lv2_descriptor(lv2_descriptor_)
{
  type = Effect::Type::lv2;
}

Effect::~Effect (void)
{

}

int
Effect::activate (size_t sample_rate) noexcept
{
  switch (type)
    {
      case Effect::Type::ladspa:
      ladspa_handle = ladspa_descriptor->instantiate(ladspa_descriptor, sample_rate);
      ladspa_descriptor->activate(ladspa_handle);
      break;

      case Effect::Type::lv2:
      break;
    }

  return EFAULT;
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

const char *
Effect::get_name (void) const noexcept
{
  switch (type)
    {
      case Effect::Type::ladspa:
      if (ladspa_descriptor == nullptr)
        return nullptr;
      return ladspa_descriptor->Name;

      case Effect::Type::lv2:
      if (lv2_descriptor == nullptr)
        return nullptr;
      return lv2_descriptor->URI;
    }

  return nullptr;
}

int
Effect::process (float *sample_array, size_t sample_count) noexcept
{
  if (ladspa_handle == nullptr)
    {
#ifdef GNUITAR_DEBUG
      std::cerr << "handle not instantiated" << std::endl;
#endif /* GNUITAR_DEBUG */
      return EFAULT;
    }

  int err = connect_to_ladspa_ports (sample_array);
  if (err != 0)
    {
#ifdef GNUITAR_DEBUG
      std::cerr << "gnuitar: failed to connect to LADSPA ports" << std::endl;
      std::cerr << "ladspa port: " << get_name() << std::endl;
#endif /* GNUITAR_DEBUG */
      return err;
    }
  ladspa_descriptor->run (ladspa_handle, sample_count);
  return 0;
}

int
Effect::connect_to_ladspa_ports (float *sample_array) noexcept
{
  bool input_connected = false;
  bool output_connected = false;
  if (ladspa_handle == nullptr)
    return EFAULT;

  decltype (ladspa_descriptor->PortCount) i;
  for (i = 0; i < ladspa_descriptor->PortCount; i++)
    {
      auto port = ladspa_descriptor->PortDescriptors[i];
      if (!LADSPA_IS_PORT_AUDIO(port))
        continue;
      else if ((!input_connected) && LADSPA_IS_PORT_INPUT(port))
        {
          ladspa_descriptor->connect_port(ladspa_handle, i, sample_array);
          input_connected = true;
	}
      else if ((!output_connected) && LADSPA_IS_PORT_OUTPUT(port))
        {
          ladspa_descriptor->connect_port(ladspa_handle, i, sample_array);
          output_connected = true;
	}
    }

  if (input_connected && output_connected)
    return 0;

  return EINVAL;
}

Plugin::Plugin (void) noexcept
{
  handle = nullptr;
}

Plugin::Plugin (const std::string& path) noexcept : Plugin()
{
  open (path);
}

Plugin::~Plugin (void)
{
  if (handle != nullptr)
    {
#ifdef _WIN32
#else /* _WIN32 */
      dlclose (handle);
#endif /* _WIN32 */
    }
}

Effect *
Plugin::get_effect (size_t index) const noexcept
{
  if (handle == nullptr)
    return nullptr;

  if (type == Plugin::Type::ladspa)
    {
      auto ladspa_effect = ladspa_descriptor(index);
      if (ladspa_effect == nullptr)
        return nullptr;
      return new Effect(ladspa_effect);
    }
  else if (type == Plugin::Type::lv2)
    {
      auto lv2_effect = lv2_descriptor(index);
      if (lv2_effect == nullptr)
        return nullptr;
      return new Effect(lv2_effect);
    }

  return nullptr;
}

bool
Plugin::good (void) const noexcept
{
  return handle != nullptr;
}

int
Plugin::open (const std::string& path) noexcept
{
#ifdef _WIN32
  (void) path;
  return EINVAL;
#else /* _WIN32 */
  auto tmp_handle = dlopen (path.c_str(), RTLD_LAZY);
  if (tmp_handle == nullptr)
    {
#ifdef GNUITAR_DEBUG
      std::cerr << "gnuitar: failed to load " << path << std::endl;
      std::cerr << "dlerror: " << dlerror() << std::endl;
#endif /* GNUITAR_DEBUG */
      return ENOENT;
    }
  if (find_entry (tmp_handle) != 0)
    {
      dlclose(tmp_handle);
      return EINVAL;
    }
  if (handle != nullptr)
    dlclose(handle);
  handle = tmp_handle;
  return 0;
#endif /* _WIN32 */
}

int
Plugin::find_entry (void *handle) noexcept
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

