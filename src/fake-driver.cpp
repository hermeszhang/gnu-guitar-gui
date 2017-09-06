#include <gnu-guitar-qt/fake-driver.hpp>

#include <array>
#include <iostream>

namespace {

const std::array<const char *, 3> masterApiList = {
  "Fake API #1",
  "Fake API #2",
  "Fake API #3"
};

const std::array<const char *, 5> masterEffectList = {
  "Fake Effect #1",
  "Fake Effect #2",
  "Fake Effect #3",
  "Fake Effect #4",
  "Fake Effect #5"
};

const std::array<const char *, 6> masterControlList = {
  "Control #1",
  "Control #2",
  "Control #3",
  "Control #4",
  "Control #5",
  "Control #6"
};

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

} // namespace

namespace GnuGuitar::Qt {

FakeDriver::FakeDriver() noexcept {

}

FakeDriver::~FakeDriver() {

}

void FakeDriver::addEffect(const std::string &effectName) {
  auto found = hasEffect(effectName);
  if (!found) {
    // THROW
  }
}

void FakeDriver::listApis(std::vector<std::string> &apiList) {
  apiList.clear();
  for (auto api : masterApiList)
    apiList.push_back(api);
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

void FakeDriver::setApi(const std::string &apiName) {

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

  api = apiName;

  std::cout << "Setting API (" << apiName << ")" << std::endl;
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

} // namespace GnuGuitar::Qt
