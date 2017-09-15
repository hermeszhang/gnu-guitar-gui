#include <gnu-guitar/gui/control.hpp>

namespace GnuGuitar::Gui {

Control::Control() {

}

Control::~Control() {

}

void Control::setName(const std::string &name_) {
  name = name_;
}

void Control::getName(std::string &name_) const {
  name_ = name;
}

} // namespace GnuGuitar::Gui
