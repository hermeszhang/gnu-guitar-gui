#include <libgnuitar/effect-list.h>

namespace Gnuitar
{

EffectList::EffectList (void) noexcept
{

}

EffectList::~EffectList (void)
{

}

void
EffectList::add (const std::string& name)
{
  names.push_back (name);
}

void
EffectList::add (std::string&& name)
{
  names.emplace_back (name);
}

std::vector<std::string>::const_iterator
EffectList::begin (void) const
{
  return names.begin ();
}

std::vector<std::string>::const_iterator
EffectList::end (void) const
{
  return names.end ();
}

} /* namespace Gnuitar */

