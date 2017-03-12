#include <libgnuitar/driver.h>
#include <libgnuitar/shared-library-collection.h>
#include <libgnuitar/effect.h>

#include <iostream>

int
main (void)
{
  Gnuitar::Driver *driver;

  driver = Gnuitar::Driver::make ();
  if (driver == nullptr)
    return EXIT_FAILURE;

  Gnuitar::SharedLibraryCollection shared_libs;
  shared_libs.add_search_path_list_from_env ("LADSPA_PATH");
  shared_libs.update ();

  LADSPA_Descriptor_Function ladspa_descriptor = nullptr;
  for (const auto& shared_lib : shared_libs)
    {
      ladspa_descriptor = (LADSPA_Descriptor_Function)(shared_lib.second.get_symbol ("ladspa_descriptor"));
      if (ladspa_descriptor == nullptr)
        continue;
    }

  const LADSPA_Descriptor* descriptor = nullptr;
  if (ladspa_descriptor != nullptr)
    descriptor = ladspa_descriptor (0);

  if (descriptor != nullptr)
    {
      auto effect = new Gnuitar::Effect (descriptor);
      effect->instantiate (driver->get_rate ());
      effect->activate ();
      driver->add_effect (effect);

      std::string name;
      effect->get_name (name);
      std::cout << "using effect: " << name << std::endl;
    }

  driver->start();

  while (true);

  delete driver;

  return EXIT_SUCCESS;
}

