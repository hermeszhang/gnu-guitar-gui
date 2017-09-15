#ifndef GNU_GUITAR_GUI_CONTROL_LIST_HPP
#define GNU_GUITAR_GUI_CONTROL_LIST_HPP

#include <vector>

namespace GnuGuitar::Gui {

class Control;
class StringControl;

class ControlList final {
  std::vector<Control *> controls;
public:
  using ConstIterator = std::vector<Control *>::const_iterator;
  ControlList();
  ~ControlList();
  void add(const StringControl &stringControl);
  ConstIterator begin() const;
  ConstIterator end() const;
};

} // namespace GnuGuitar::Gui

#endif // GNU_GUITAR_GUI_CONTROL_LIST_HPP
