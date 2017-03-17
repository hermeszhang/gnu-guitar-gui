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

float
Driver::get_control_value (const std::string&)
{
  return 0.0f;
}

void
Driver::set_control_value (const std::string& effect_name, const std::string& control_name, float control_value)
{
  for (Effect *effect : effects)
    {
      if (effect->get_name () == effect_name)
        effect->set_control_value (control_name, control_value);
    }
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

