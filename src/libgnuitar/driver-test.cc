#include <libgnuitar/driver.h>

#include <iostream>

namespace
{

} /* namespace */

int
main (void)
{
  Gnuitar::Driver *driver;

  driver = Gnuitar::Driver::make ();
  if (driver == nullptr)
    return EXIT_FAILURE;

  driver->set_input("plughw:1,0");

  Gnuitar::LADSPA::PluginManager plugin_manager;
  plugin_manager.parse_env();
  plugin_manager.find_all_plugins();
  auto effect = plugin_manager.get_effect("C* Scape - Stereo delay with chromatic resonances");
  if (effect != nullptr)
    {
      std::cout << "using effect: " << effect->get_name () << std::endl;
      effect->instantiate(driver->get_rate());
      effect->activate();
      driver->add_effect(effect);
    }

  driver->start();

  while (true);

  delete driver;

  return EXIT_SUCCESS;
}

