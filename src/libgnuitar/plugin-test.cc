#include <libgnuitar/plugin.h>

#include <cstdlib>

#include <iostream>

namespace
{

void
view_plugin (const std::string& path)
{
  Gnuitar::LADSPA::Plugin plugin (path);
  if (!plugin.good ())
    {
      std::cerr << "failed to open '" << path << "'" << std::endl;
      return;
    }
  for (size_t i = 0; true; i++)
    {
      auto effect = plugin.get_effect (i);
      if (effect == nullptr)
        break;
      std::cout << "effect: " << effect->get_name () << std::endl;
      std::cout << "  maker: " << effect->get_maker () << std::endl;
      delete effect;
    }
}

} /* namespace */

int
main (int argc, const char **argv)
{
  for (decltype (argc) i = 1; i < argc; i++)
    view_plugin (argv[i]);
  return EXIT_SUCCESS;
}

