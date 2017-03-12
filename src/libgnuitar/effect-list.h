#ifndef GNUITAR_EFFECT_LIST_H
#define GNUITAR_EFFECT_LIST_H

#include <string>
#include <vector>

namespace Gnuitar
{

class EffectList final
{
  std::vector<std::string> names;
public:
  EffectList (void) noexcept;
  ~EffectList (void);
  void add(const std::string& name);
  void add(std::string&& name);
  std::vector<std::string>::const_iterator begin (void) const;
  std::vector<std::string>::const_iterator end (void) const;
}; /* class EffectList */

} /* namespace Gnuitar */

#endif /* GNUITAR_EFFECT_LIST_H */

