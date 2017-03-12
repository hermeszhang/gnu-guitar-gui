#include <libgnuitar/effect.h>

namespace Gnuitar
{

Effect::Effect (const LADSPA_Descriptor *descriptor_) noexcept : descriptor(descriptor_)
{

}

Effect::~Effect (void)
{
  if ((handle != nullptr)
   && (descriptor != nullptr)
   && (descriptor->cleanup != nullptr))
    descriptor->cleanup(handle);
}

bool
Effect::instantiate (unsigned long rate) noexcept
{
  if ((descriptor == nullptr)
   || (descriptor->instantiate == nullptr))
    return false;

  handle = descriptor->instantiate (descriptor, rate);
  if (handle == nullptr)
    return false;

  return true;
}

bool
Effect::activate (void) noexcept
{
  if ((descriptor == nullptr)
   || (descriptor->activate == nullptr)
   || (handle == nullptr))
    return false;

  descriptor->activate(handle);

  return true;
}

bool
Effect::connect (float *sample_array) noexcept
{
  if ((descriptor == nullptr)
   || (descriptor->connect_port == nullptr)
   || (handle == nullptr))
    return false;

  bool input_found = false;
  bool output_found = false;
  for (auto i = 0ULL; i < descriptor->PortCount; i++)
    {
      auto port = descriptor->PortDescriptors[i];
      if (!LADSPA_IS_PORT_AUDIO (port))
        continue;
      else if (LADSPA_IS_PORT_INPUT (port))
        {
          input_found = true;
          descriptor->connect_port(handle, i, sample_array);
        }
      else if (LADSPA_IS_PORT_OUTPUT (port))
        {
          output_found = true;
          descriptor->connect_port(handle, i, sample_array);
        }
    }

  if (input_found && output_found)
    return true;

  return false;
}

bool
Effect::run (size_t sample_count) noexcept
{
  if ((descriptor == nullptr)
   || (descriptor->run == nullptr)
   || (handle == nullptr))
    return false;

  descriptor->run (handle, sample_count);

  return true;
}

bool
Effect::get_name (std::string& name) const noexcept
{
  if ((descriptor == nullptr)
   || (descriptor->Name == nullptr))
    return false;
  name = descriptor->Name;
  return true;
}

} /* namespace Gnuitar */

