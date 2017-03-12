#include <libgnuitar/driver.h>

#ifdef GNUITAR_WITH_ALSA
#include <libgnuitar/alsa-driver.h>
#endif /* GNUITAR_WITH_ALSA */

#include <libgnuitar/exception.h>

namespace Gnuitar {

Driver *
Driver::make (void)
{
#ifdef GNUITAR_WITH_ALSA
  return new AlsaDriver;
#endif /* GNUITAR_WITH_ALSA */
  throw Exception ("no audio drivers compiled");
  /* unreachable */
  return nullptr;
}

Driver::~Driver (void)
{
  for (Effect *effect : effects)
    {
      if (effect == nullptr)
        continue;
      delete effect;
    }
}

void
Driver::add_effect (Effect *effect)
{
  if (effect == nullptr)
    throw std::invalid_argument ("effect is null");
  effects.push_back(effect);
}

void
Driver::connect (float *sample_array)
{
  for (Effect *effect : effects)
    {
      if (effect == nullptr)
        continue;
      effect->connect (sample_array);
    }
}

void
Driver::run (size_t sample_count)
{
  for (Effect *effect : effects)
    {
      if (effect == nullptr)
        continue;
      effect->run (sample_count);
    }
}

} /* namespace Gnuitar */

