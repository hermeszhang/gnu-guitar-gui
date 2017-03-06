#include <gnuitar/driver.h>

#include <iostream>

namespace
{

} /* namespace */

int
main(void)
{
  Gnuitar::Driver *driver;

  driver = Gnuitar::Driver::make ();
  if (driver == nullptr)
    return EXIT_FAILURE;

#ifdef __unix__
  Gnuitar::Plugin plugin("plugins/caps.so");
  if (plugin.opened())
    {
      auto effect = plugin.get_effect(0);
      if (effect != nullptr)
        {
          std::cout << "using effect: " << effect->get_name () << std::endl;
          effect->activate(48000);
          driver->add_effect(effect);
        }
    }
#endif /* __unix__ */

  driver->start();

  while (true);

  delete driver;

  return EXIT_SUCCESS;
}

