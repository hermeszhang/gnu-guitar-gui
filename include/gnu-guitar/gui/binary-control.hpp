#ifndef GNU_GUITAR_GUI_BINARY_CONTROL_HPP
#define GNU_GUITAR_GUI_BINARY_CONTROL_HPP

#include <gnu-guitar/gui/control.hpp>

namespace GnuGuitar {

namespace Gui {

class BinaryControl final : public Control {
  bool state;
public:
  BinaryControl();
  ~BinaryControl();
  void accept(ControlVisitor &controlVistor) const override;
  bool off() const;
  bool on() const;
  void setOn();
  void setOff();
};

} // namespace Gui

} // namespace GnuGuitar

#endif // GNU_GUITAR_GUI_BINARY_CONTROL_HPP
