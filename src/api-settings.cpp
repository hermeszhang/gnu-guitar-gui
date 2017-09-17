#include <gnu-guitar/gui/api-settings.hpp>

#include <gnu-guitar/gui/control.hpp>
#include <gnu-guitar/gui/control-list.hpp>
#include <gnu-guitar/gui/control-visitor.hpp>

namespace GnuGuitar::Gui {

ApiSettings::ApiSettings() {
  controlList = new ControlList;
}

ApiSettings::ApiSettings(ApiSettings &&other) {
  apiName = std::move(other.apiName);
  controlList = other.controlList;
  other.controlList = nullptr;
}

ApiSettings::~ApiSettings() {
  if (controlList != nullptr) {
    delete controlList;
    controlList = nullptr;
  }
}

void ApiSettings::accept(ControlVisitor &controlVisitor) const {
  for (auto control : *controlList) {
     control->accept(controlVisitor);
  }
}

void ApiSettings::addControl(const BinaryControl &binaryControl) {
  controlList->add(binaryControl);
}

void ApiSettings::addControl(const StringControl &stringControl) {
  controlList->add(stringControl);
}

void ApiSettings::getApiName(std::string &apiName_) const {
  apiName_ = apiName;
}

void ApiSettings::setApiName(const std::string &apiName_) {
  apiName = apiName_;
}

} // namespace GnuGuitar::Gui

