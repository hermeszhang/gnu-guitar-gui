#ifndef GNU_GUITAR_GUI_CONTROL_HPP
#define GNU_GUITAR_GUI_CONTROL_HPP

#include <string>

namespace GnuGuitar::Gui {

class ControlVisitor;

class Control {
  std::string name;
public:
  Control();
  virtual ~Control();
  virtual void accept(ControlVisitor &controlVisitor) const = 0;
  virtual void getName(std::string &name) const;
  virtual void setName(const std::string &name);
};

} // namespace GnuGuitar::Gui

#endif // GNU_GUITAR_GUI_CONTROL_HPP
