#include <gnu-guitar/gui/string-control.hpp>

#include <gnu-guitar/gui/control-visitor.hpp>

namespace GnuGuitar::Gui {

StringControl::StringControl() {

}

StringControl::~StringControl() {

}

void StringControl::accept(ControlVisitor &controlVisitor) {
  controlVisitor.visit(*this);
}

void StringControl::addOption(const std::string &option) {
  options.push_back(option);
}

void StringControl::getValue(std::string &value_) const {
  value_ = value;
}

void StringControl::setValue(const std::string &value_) {
  value = value_;
}

} // namespace GnuGuitar::Gui
