#include <gnu-guitar/gui/binary-control.hpp>

#include <gnu-guitar/gui/control-visitor.hpp>

namespace GnuGuitar {

namespace Gui {

BinaryControl::BinaryControl() {

}

BinaryControl::~BinaryControl() {

}

void BinaryControl::accept(ControlVisitor &controlVisitor) const {
  controlVisitor.visit(*this);
}

bool BinaryControl::on() const {
  return state;
}

bool BinaryControl::off() const {
  return !state;
}

void BinaryControl::setOn() {
  state = true;
}

void BinaryControl::setOff() {
  state = false;
}

} // namespace Gui

} // namespace GnuGuitar

