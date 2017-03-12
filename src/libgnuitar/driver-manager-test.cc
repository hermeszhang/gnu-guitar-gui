#include <libgnuitar/driver-manager.h>

#include <iostream>

int
main (void)
{
  Gnuitar::DriverManager driver_manager;

  driver_manager.update_libs ();

  std::cout << "effects discovered:" << std::endl;

  auto effect_list = driver_manager.get_effect_list ();
  for (const auto& effect_name : effect_list)
    std::cout << "  " << effect_name << std::endl;

  std::cout << "use effect: " << std::endl;
  std::string effect_name;
  std::getline (std::cin, effect_name);
  if (effect_name != "")
    driver_manager.add_effect (effect_name);
  driver_manager.start_driver ();

  while (true);

  return 0;
}

