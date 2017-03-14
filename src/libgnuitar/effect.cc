#include <libgnuitar/effect.h>
#include <libgnuitar/exception.h>

#include <climits>

namespace
{

bool
init_control (const LADSPA_Descriptor *descriptor, unsigned long i, Gnuitar::Control& control)
{
  auto port = descriptor->PortDescriptors[i];
  if (!LADSPA_IS_PORT_CONTROL (port))
    return false;

  if (descriptor->PortNames[i] != nullptr)
    control.set_label (descriptor->PortNames[i]);

  return true;
}

Gnuitar::ControlSet
init_control_set (const LADSPA_Descriptor *descriptor)
{
  Gnuitar::ControlSet control_set;
  if (descriptor == nullptr)
    return control_set;
  else if (descriptor->PortNames == nullptr)
    return control_set;
  else if (descriptor->PortDescriptors == nullptr)
    return control_set;

  for (auto i = 0UL; i < descriptor->PortCount; i++)
    {
      Gnuitar::Control control;
      if (init_control (descriptor, i, control))
        control_set.add (std::move (control));
    }
  return control_set;
}

} /* namespace */

namespace Gnuitar
{

Effect::Effect (const LADSPA_Descriptor *descriptor_) noexcept : descriptor(descriptor_)
{
  if (descriptor != nullptr)
    {
      if (descriptor->Name != nullptr)
        name = descriptor->Label;
    }
}

Effect::~Effect (void)
{
  if ((handle != nullptr)
   && (descriptor != nullptr)
   && (descriptor->cleanup != nullptr))
    descriptor->cleanup(handle);
}

const ControlSet&
Effect::get_control_set (void) const
{
  return control_set;
}

void
Effect::instantiate (unsigned long rate)
{
  if (descriptor == nullptr)
    throw Exception ("LADSPA descriptor is null");
  else if (descriptor->instantiate == nullptr)
    throw Exception ("LADSPA instantiation function is null");

  handle = descriptor->instantiate (descriptor, rate);
  if (handle == nullptr)
    throw Exception ("failed to instantiate LADSPA handle");

  control_set = init_control_set (descriptor);
}

void
Effect::activate (void)
{
  if (descriptor == nullptr)
    throw Exception ("LADSPA descriptor is null");
  else if (descriptor->activate == nullptr)
    throw Exception ("LADSPA activation function is null");
  else if (handle == nullptr)
    throw Exception ("LADSPA handle is null");

  descriptor->activate(handle);
}

void
Effect::connect (float *sample_array)
{
  if (descriptor == nullptr)
    throw Exception ("LADSPA descriptor is null");
  else if (descriptor->connect_port == nullptr)
    throw Exception ("LADSPA port connection function is null");
  else if (handle == nullptr)
    throw Exception ("LADSPA handle is null");

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

  if (!input_found)
    throw Exception ("LADSPA descriptor has no input port");
  else if (!output_found)
    throw Exception ("LADSPA descriptor has no output port");
}

void
Effect::run (size_t sample_count)
{
  if (descriptor == nullptr)
    throw Exception ("LADSPA descriptor is null");
  else if (descriptor->run == nullptr)
    throw Exception ("LADSPA run function is null");
  else if (handle == nullptr)
    throw Exception ("LADSPA handle is null");

  descriptor->run (handle, sample_count);
}

const std::string&
Effect::get_name (void) const
{
  return name;
}

} /* namespace Gnuitar */

