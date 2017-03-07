#include <libgnuitar/plugin.h>

#include <cerrno>

#ifdef GNUITAR_DEBUG
#include <iostream>
#endif /* GNUITAR_DEBUG */

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <glob.h>
#endif /* _WIN32 */

namespace Gnuitar {

Effect::~Effect (void)
{

}

Plugin::Plugin (Plugin&& plugin) noexcept
{
  handle = std::move(plugin.handle);
  plugin.handle = nullptr;
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

bool
Plugin::good (void) const noexcept
{
  return handle != nullptr;
}

bool
Plugin::open (const std::string& path) noexcept
{
#ifdef _WIN32
  (void) path;
  return false;
#else /* _WIN32 */
  /* open a new handle, in a
   * temporary variable (in case it fails) */
  auto tmp_handle = dlopen (path.c_str(), RTLD_LAZY);
  if (tmp_handle == nullptr)
    {
#ifdef GNUITAR_DEBUG
      std::cerr << "gnuitar: failed to load " << path << std::endl;
      std::cerr << "dlerror: " << dlerror() << std::endl;
#endif /* GNUITAR_DEBUG */
      return false;
    }
  /* close the previous handle,
   * if it was opened */
  if (handle != nullptr)
    dlclose(handle);
  /* set new handle */
  handle = tmp_handle;
  return true;
#endif /* _WIN32 */
}

void *
Plugin::get_symbol (const std::string& symbol_name) noexcept
{
#ifdef _WIN32
  return nullptr;
#else /* _WIN32 */
  if (handle == nullptr)
    return nullptr;
  return dlsym(handle, symbol_name.c_str());
#endif /* _WIN32 */
}

namespace LADSPA
{

Effect::Effect (const LADSPA_Descriptor *descriptor_) noexcept : descriptor(descriptor_)
{
  handle = nullptr;
}

Effect::~Effect (void) noexcept
{
  if ((handle != nullptr)
   && (descriptor != nullptr)
   && (descriptor->cleanup != nullptr))
    descriptor->cleanup(handle);
}

bool
Effect::instantiate (size_t sample_rate) noexcept
{
  if (descriptor == nullptr)
    return false;

  /* if handle is already open,
   * exit early */
  if (handle != nullptr)
    return true;

  handle = descriptor->instantiate(descriptor, sample_rate);
  if (handle == nullptr)
    return false;

  return true;
}

bool
Effect::activate (void) noexcept
{
  if ((handle == nullptr)
   || (descriptor == nullptr)
   || (descriptor->activate == nullptr))
    return false;

  descriptor->activate(handle);

  return true;
}

bool
Effect::deactivate (void) noexcept
{
  if ((handle == nullptr)
   || (descriptor == nullptr)
   || (descriptor->deactivate == nullptr))
    return false;

  descriptor->deactivate(handle);

  return true;
}

std::string
Effect::get_maker (void) const noexcept
{
  std::string maker;

  /* assign the maker, if it's not null */
  if ((descriptor != nullptr)
   && (descriptor->Maker))
    maker = descriptor->Maker;

  return maker;
}

std::string
Effect::get_name (void) const noexcept
{
  std::string name;

  if ((descriptor != nullptr)
   && (descriptor->Name != nullptr))
    name = descriptor->Name;

  return name;
}

bool
Effect::connect (float *sample_array) noexcept
{
  if ((handle == nullptr)
   || (descriptor == nullptr)
   || (descriptor->connect_port == nullptr))
    return false;

  bool input_found = false;
  bool output_found = false;
  for (size_t i = 0; i < descriptor->PortCount; i++)
    {
      auto port = descriptor->PortDescriptors[i];
      if (!LADSPA_IS_PORT_AUDIO(port))
        continue;
      else if (LADSPA_IS_PORT_INPUT(port))
        {
          descriptor->connect_port(handle, i, sample_array);
          input_found = true;
        }
      else if (LADSPA_IS_PORT_OUTPUT(port))
        {
          descriptor->connect_port(handle, i, sample_array);
          output_found = true;
        }
      if (input_found && output_found)
        return true;
    }
  return false;
}

void
Effect::run (size_t sample_count) noexcept
{
  if ((handle == nullptr)
   || (descriptor == nullptr)
   || (descriptor->run == nullptr))
    return;

  descriptor->run (handle, sample_count);
}

Plugin::Plugin (Plugin&& plugin) noexcept : Gnuitar::Plugin(std::move(plugin))
{
  descriptor_function = std::move(plugin.descriptor_function);
}

Plugin::Plugin (void) noexcept
{
  descriptor_function = nullptr;
}

Plugin::Plugin (const std::string& path) noexcept : Plugin()
{
  open(path);
}

Plugin::~Plugin (void)
{

}

bool
Plugin::good (void) const noexcept
{
  if (!Gnuitar::Plugin::good())
    return false;
  else if (descriptor_function == nullptr)
    return false;
  return true;
}

bool
Plugin::open (const std::string& path) noexcept
{
  if (!Gnuitar::Plugin::open(path))
    return false;

  descriptor_function = (LADSPA_Descriptor_Function)(get_symbol("ladspa_descriptor"));
  if (descriptor_function == nullptr)
    return false;

  return true;
}

LADSPA::Effect *
Plugin::get_effect (size_t index) const noexcept
{
  if (!good())
    return nullptr;

  auto descriptor = descriptor_function(index);
  if (descriptor == nullptr)
    return nullptr;

  return new LADSPA::Effect(descriptor);
}

PluginManager::PluginManager (void) noexcept
{
#ifdef __unix__
  add_path("/usr/lib/ladspa");
  add_path("/usr/local/lib/ladspa");
#endif /* __unix__ */
}

PluginManager::~PluginManager (void)
{

}

bool
PluginManager::open_by_name (const std::string& name) noexcept
{
  std::string filename;
#ifdef _WIN32
  filename = name + ".dll";
#else /* _WIN32 */
  filename = name + ".so";
#endif /* _WIN32 */

  for (const std::string& path : paths)
    {
      if (open_by_path (path + "/" + filename) == 0)
        return true;
    }

  return false;
}

bool
PluginManager::open_by_path (const std::string& path) noexcept
{
  Plugin plugin (path);
  if (plugin.good ())
    {
      plugins.emplace_back(std::move(plugin));
      return true;
    }
  return false;
}

bool
PluginManager::add_path (const std::string& path) noexcept
{
  if (path.size() == 0)
    return EINVAL;

  /* check if path already exists */
  for (const std::string& path_entry : paths)
    {
      if (path_entry == path)
        return true;
    }

  paths.push_back(path);

  return true;
}

bool
PluginManager::find_all_plugins (void) noexcept
{
  for (const std::string& path : paths)
    {
      find_all_plugins(path);
    }
  return true;
}

bool
PluginManager::find_all_plugins (const std::string& path) noexcept
{
#ifdef _WIN32
  (void) path;
  return false;
#else /* _WIN32 */

  std::string pattern;
  pattern = path + "/*.so";

  glob_t glbuf;

  if (glob(pattern.c_str(), 0, nullptr, &glbuf) != 0)
    return false;

  for (decltype(glbuf.gl_pathc) i = 0; i < glbuf.gl_pathc; i++)
    {
      open_by_path(glbuf.gl_pathv[i]);
    }

  globfree(&glbuf);

  return true;
#endif /* _WIN32 */
}

Effect *
PluginManager::get_effect (const std::string& name) noexcept
{
  if (name.size() == 0)
    return nullptr;

  for (const Plugin& plugin : plugins)
    {
      for (size_t i = 0; i < SIZE_MAX; i++)
        {
          auto effect = plugin.get_effect(i);
          if (effect == nullptr)
            break;

          auto effect_name = effect->get_name();
          if (effect_name == name)
            return effect;

          delete effect;
        }
    }
  return nullptr;
}

bool
PluginManager::parse_paths (const std::string& paths) noexcept
{
  std::string tmp_path;
  for (auto c : paths)
    {
      if (c == ':')
        {
          add_path (tmp_path);
          tmp_path.clear ();
        }
      else
        tmp_path.push_back (c);
    }

  if (tmp_path.size () > 0)
    add_path (tmp_path);

  return true;
}

bool
PluginManager::parse_env (void) noexcept
{
  std::string ladspa_paths = std::getenv ("LADSPA_PATH");
  if (ladspa_paths.size () == 0)
    return false;
  return parse_paths (ladspa_paths);
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
  return plugins.size ();
}

} /* namespace LADSPA */

} /* namespace Gnuitar */

