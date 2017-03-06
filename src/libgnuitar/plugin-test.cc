#include <gnuitar/plugin.h>

#include <cstdlib>

#include <iostream>

namespace
{

void
view_plugin (const char *path)
{
  Gnuitar::Plugin plugin(path);

  for (size_t i = 0; true; i++)
  {
    auto effect = plugin.get_effect(i);
    if (effect == nullptr)
      break;

    auto maker = effect->get_maker();
    if (maker == nullptr)
      continue;

    std::cout << "maker: " << maker << std::endl;

    delete effect;
  }
}

} /* namespace */

int
main (int argc, const char **argv)
{
  for (decltype(argc) i = 1; i < argc; i++)
  {
    view_plugin(argv[i]);
  }
  return EXIT_SUCCESS;
}

