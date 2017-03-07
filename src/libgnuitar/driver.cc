#include <libgnuitar/driver.h>

#ifdef GNUITAR_WITH_ALSA
#include "alsa-driver.h"
#endif /* GNUITAR_WITH_ALSA */

namespace Gnuitar {

Driver *
Driver::make (void) noexcept
{
#ifdef GNUITAR_WITH_ALSA
  return new ALSA::Driver;
#endif /* GNUITAR_WITH_ALSA */
  return nullptr;
}

Driver::~Driver (void)
{
  for (Effect *effect : effects)
    delete effect;
}

int
Driver::add_effect (Effect *effect) noexcept
{
  effects.push_back(effect);
  return 0;
}

bool
Driver::connect (float *sample_array) noexcept
{
  for (Effect *effect : effects)
    {
      if (!effect->connect (sample_array))
        return false;
    }
  return true;
}

void
Driver::run (size_t sample_count) noexcept
{
  for (Effect *effect : effects)
    {
      effect->run (sample_count);
    }
}

} /* namespace Gnuitar */

