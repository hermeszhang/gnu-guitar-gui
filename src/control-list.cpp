#include <gnu-guitar/gui/control-list.hpp>

#include <gnu-guitar/gui/string-control.hpp>

namespace GnuGuitar::Gui {

ControlList::ControlList() {

}

ControlList::ControlList(ControlList &&other) {
  controls = std::move(other.controls);
}

ControlList::~ControlList() {
  for (auto control : controls)
    delete control;
}

void ControlList::add(const StringControl &stringControl) {
  controls.push_back(new StringControl(stringControl));
}

ControlList::ConstIterator ControlList::begin() const {
  return controls.begin();
}

ControlList::ConstIterator ControlList::end() const {
  return controls.end();
}

} // namespace GnuGuitar::Gui
