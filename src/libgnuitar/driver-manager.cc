#include <libgnuitar/driver-manager.h>

#include <libgnuitar/exception.h>

#include <climits>

namespace Gnuitar
{

DriverManager::DriverManager (void) noexcept
{
  driver = Driver::make ();
  shared_libs.add_search_path_list_from_env ("LADSPA_PATH");
  shared_libs.add_search_path_list_from_env ("LV2_PATH");
}

DriverManager::~DriverManager (void)
{

}

Effect *
DriverManager::add_effect (const std::string& name)
{
  if (driver == nullptr)
    throw std::invalid_argument ("driver is null");

  auto ladspa_descriptor = search_ladspa_descriptor (name);
  if (ladspa_descriptor == nullptr)
    throw Exception ("effect not found");

  auto effect = new Effect (ladspa_descriptor);
  effect->instantiate (driver->get_rate ());
  effect->activate ();

  driver->add_effect (effect);

  return effect;
}

EffectList
DriverManager::get_effect_list (void) const
{
  EffectList effect_list;
  for (const auto& shared_lib : shared_libs)
    {
      auto init_func = (LADSPA_Descriptor_Function)(shared_lib.second.get_symbol("ladspa_descriptor"));
      if (init_func == nullptr)
        break;

      for (auto i = 0UL; i < ULONG_MAX; i++)
        {
          auto descriptor = init_func (i);
          if (descriptor == nullptr)
            break;
          else if (descriptor->Name == nullptr)
            continue;

          std::string name (descriptor->Label);

          effect_list.add (std::move (name));
        }
    }

  return effect_list;
}

void
DriverManager::start_driver (void)
{
  if (driver == nullptr)
    throw std::invalid_argument ("driver is null");
  driver->start ();
}

void
DriverManager::stop_driver (void)
{
  if (driver == nullptr)
    throw std::invalid_argument ("driver is null");
  driver->stop ();
}

void
DriverManager::update_libs (void) noexcept
{
  shared_libs.update ();
}

const LADSPA_Descriptor *
DriverManager::search_ladspa_descriptor (const std::string& name) noexcept
{
  for (const auto& shared_lib : shared_libs)
    {
      auto init_func = (LADSPA_Descriptor_Function)(shared_lib.second.get_symbol("ladspa_descriptor"));
      if (init_func == nullptr)
        break;

      for (auto i = 0UL; i < ULONG_MAX; i++)
        {
          auto descriptor = init_func (i);
          if (descriptor == nullptr)
            break;
          else if (descriptor->Name == nullptr)
            continue;

          if (name == descriptor->Label)
            return descriptor;
        }
    }
  return nullptr;
}

} /* namespace Gnuitar */

