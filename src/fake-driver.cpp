#include <gnu-guitar/gui/fake-driver.hpp>

#include <gnu-guitar/gui/api-settings.hpp>
#include <gnu-guitar/gui/binary-control.hpp>
#include <gnu-guitar/gui/control-visitor.hpp>
#include <gnu-guitar/gui/string-control.hpp>

#include <array>
#include <iostream>

namespace {

const std::array<const char *, 3> masterApiList = { {
  "Fake API #1",
  "Fake API #2",
  "Fake API #3"
} };

const std::array<const char *, 5> masterEffectList = { {
  "Fake Effect #1",
  "Fake Effect #2",
  "Fake Effect #3",
  "Fake Effect #4",
  "Fake Effect #5"
} };

const std::array<const char *, 6> masterControlList = { {
  "Control #1",
  "Control #2",
  "Control #3",
  "Control #4",
  "Control #5",
  "Control #6"
} };

const std::array<const char *, 3> masterApiControlList = { {
  "Api Control #1",
  "Api Control #2",
  "Api Control #3",
} };

bool hasControl(const std::string &controlName) {
  for (auto control : masterControlList) {
    if (controlName == control)
      return true;
  }
  return false;
}

bool hasEffect(const std::string &effectName) {
  auto found = false;
  for (auto effect : masterEffectList) {
    if (effectName == effect) {
      found = true;
      break;
    }
  }
  return found;
}

class ControlPrinter final : public GnuGuitar::Gui::ControlVisitor {
  std::ostream &out;
public:
  ControlPrinter(std::ostream &out_) : out(out_) {

  }
  ~ControlPrinter() {

  }
  void visit(const GnuGuitar::Gui::BinaryControl &binaryControl) {

    std::string name;
    bool value;

    binaryControl.getName(name);

    value = binaryControl.on();

    out << "Binary control:" << std::endl;
    out << "  Name: " << name << std::endl;
    out << "  State: " << value << std::endl;
  }
  void visit(const GnuGuitar::Gui::StringControl &stringControl) {

    std::string name;
    std::string value;

    stringControl.getName(name);
    stringControl.getValue(value);

    out << "String control:" << std::endl;
    out << "  Name: " << name << std::endl;
    out << "  Value: " << value << std::endl;
  }
};

} // namespace

namespace GnuGuitar {

namespace Gui {

FakeDriver::FakeDriver() noexcept {

}

FakeDriver::~FakeDriver() {

}

void FakeDriver::addEffect(const std::string &effectName) {
  auto found = hasEffect(effectName);
  if (!found) {
    // THROW
  } else {
    effectList.push_back(effectName);
  }
}

void FakeDriver::listApis(std::vector<ApiSettings> &apiList) {
  apiList.clear();
  for (auto api : masterApiList) {

    Gui::ApiSettings apiSettings;
    apiSettings.setApiName(api);

    Gui::StringControl controlOne;
    controlOne.setName("Control One");
    controlOne.addOption("Value 1");
    controlOne.addOption("Value 2");
    controlOne.addOption("Value 3");
    apiSettings.addControl(controlOne);

    apiList.push_back(std::move(apiSettings));
  }
}

void FakeDriver::listEffects(std::vector<std::string> &effectList) {
  effectList.clear();
  for (auto effect : masterEffectList)
    effectList.push_back(effect);
}

void FakeDriver::listEffectControls(const std::string &effectName,
                                    std::vector<std::string>& controlList) {
  if (!hasEffect(effectName)) {
    // THROW
    return;
  } else if (!usingEffect(effectName)) {
    // THROW
    return;
  }
  for (auto control : masterControlList)
    controlList.push_back(control);
}

void FakeDriver::setApi(const ApiSettings &apiSettings) {

  std::string apiName;
  apiSettings.getApiName(apiName);

  auto found = false;
  for (auto api_ : masterApiList) {
    if (apiName == api_) {
      found = true;
      break;
    }
  }
  if (!found) {
    // THROW
    return;
  }

  std::cout << "Setting API (" << apiName << ")" << std::endl;

  ::ControlPrinter controlPrinter(std::cout);

  apiSettings.accept(controlPrinter);

  api = apiName;
}

void FakeDriver::setEffectControlValue(const std::string &effectName,
                                       const std::string &controlName,
                                       float value) {
  if (!hasEffect(effectName)) {
    // THROW
    return;
  } else if (!usingEffect(effectName)) {
    // THROW
    return;
  } else if (!hasControl(controlName)) {
    // THROW
    return;
  }
  std::cout << "Setting effect (";
  std::cout << effectName;
  std::cout << ") control (";
  std::cout << controlName;
  std::cout << ") to ";
  std::cout << value << std::endl;
}

void FakeDriver::start() {
  std::cout << "Started the driver" << std::endl;
}

void FakeDriver::stop() {
  std::cout << "Stopping the driver" << std::endl;
}

bool FakeDriver::usingApi(const std::string &apiName) {
  if (apiName == api)
    return true;
  else
    return false;
}

bool FakeDriver::usingEffect(const std::string &effectName) {
  for (auto effect : effectList) {
    if (effect == effectName)
      return true;
  }
  return false;
}

} // namespace Gui

} // namespace GnuGuitar
