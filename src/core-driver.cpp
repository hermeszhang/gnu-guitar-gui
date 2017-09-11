#include <gnu-guitar-qt/core-driver.hpp>

#include <gnu-guitar-qt/ladspa-setup.hpp>

#include <gnu-guitar-core/composite-processor.hpp>
#include <gnu-guitar-core/ladspa-plugins.hpp>
#include <gnu-guitar-core/ladspa-port.hpp>
#include <gnu-guitar-core/ladspa-processor.hpp>
#include <gnu-guitar-core/processor-visitor.hpp>
#include <gnu-guitar-core/session.hpp>

#include <sstream>
#include <ostream>

namespace {

class ControlLister final : public GnuGuitar::Core::ProcessorVisitor {
  bool effectFound;
  std::string effectName;
  std::vector<std::string> &controlList;
public:
  ControlLister(std::vector<std::string> &controlList_) : controlList(controlList_) {
    effectFound = false;
  }
  ~ControlLister() {
  }
  const std::vector<std::string> &getControlList() const {
    return controlList;
  }
  void setEffectName(const std::string &name) {
    effectName = name;
  }
  void visit(GnuGuitar::Core::CompositeProcessor &compositeProcessor) {
    if (effectFound)
      return;
    for (auto processor : compositeProcessor)
      processor->accept(*this);
  }
  void visit(GnuGuitar::Core::LadspaProcessor &ladspaProcessor) {
    if (effectFound)
      return;
    else if (ladspaProcessor.getName() == effectName) {
      ladspaProcessor.listControls(controlList);
      effectFound = true;
    }
  }
};

class ControlUpdater final : public GnuGuitar::Core::ProcessorVisitor {
  /// @brief The name of the control
  ///  to update.
  std::string controlName;
  /// @brief The name of the effect
  ///  to update.
  std::string effectName;
  /// @brief The new value to set the
  ///  control to. This value should be
  ///  between 0.0 and 1.0 because it is
  ///  used as a ratio of the min and max
  ///  values of the control.
  float controlValue;
public:
  void setControlName(const std::string &name) {
    controlName = name;
  }
  void setControlValue(float value) {
    if (value > 1.0f)
      controlValue = 1.0f;
    else if (value < 0.0f)
      controlValue = 0.0f;
    else
      controlValue = value;
  }
  void setEffectName(const std::string &name) {
    effectName = name;
  }
  void visit(GnuGuitar::Core::CompositeProcessor &processor) {
    for (auto processor : processor)
      processor->accept(*this);
  }
  void visit(GnuGuitar::Core::LadspaProcessor &processor) {

    // TODO : throw exception if a control is not found

    if (processor.getName() != effectName)
      return;

    auto functor = [&](GnuGuitar::Core::LadspaControl &control) {
      control.value = (controlValue * (control.value_max - control.value_min)) + control.value_min;
    };

    GnuGuitar::Core::LadspaControlFinder<decltype(functor)> controlFinder(controlName, functor);

    processor.acceptPortVisitor(controlFinder);
  }
};

} // namespace

namespace GnuGuitar {

namespace Qt {

CoreDriver::CoreDriver() {
  ladspaPlugins = new Core::LadspaPlugins;
  ladspaPlugins->addDefaultSearchPaths();
  ladspaPlugins->update();
  processor = new Core::CompositeProcessor;
  session = new Core::Session(Core::ApiSpecifier::Any);
  session->setProcessor(processor);
}

CoreDriver::~CoreDriver() {
  if (session != nullptr) {
    delete session;
    session = nullptr;
  }
  if (processor != nullptr) {
    delete processor;
    processor = nullptr;
  }
  if (ladspaPlugins != nullptr) {
    delete ladspaPlugins;
    ladspaPlugins = nullptr;
  }
}

void CoreDriver::addEffect(const std::string &effectName) {
  auto ladspaProcessor = new Core::LadspaProcessor;
  auto findSuccess = ladspaPlugins->find(effectName, *ladspaProcessor);
  if (!findSuccess) {
    // TODO : throw an exception
    return;
  }
  ladspaProcessor->instantiate(48000);
  ladspaProcessor->activate();

  LadspaSetup ladspaSetup;

  std::vector<std::string> inputList;
  ladspaProcessor->listInputs(inputList);
  for (const auto &input : inputList)
    ladspaSetup.addInput(input);

  std::vector<std::string> outputList;
  ladspaProcessor->listOutputs(outputList);
  for (const auto &output : outputList)
    ladspaSetup.addOutput(output);

  ladspaSetup.exec();

  if (ladspaSetup.cancelled()) {
    delete ladspaProcessor;
    return;
  }

  ladspaProcessor->selectInput(ladspaSetup.getInputName());
  ladspaProcessor->selectOutput(ladspaSetup.getOutputName());

  processor->append(ladspaProcessor);
}

void CoreDriver::listApis(std::vector<std::string> &apiList) {
  std::vector<Core::ApiSpecifier> apiSpecifierList;
  Core::listCompiledApis(apiSpecifierList);
  for (auto apiSpecifier : apiSpecifierList) {
    std::stringstream apiName;
    apiName << apiSpecifier;
    apiList.emplace_back(std::move(apiName.str()));
  }
}

void CoreDriver::listEffects(std::vector<std::string> &effectList) {
  ladspaPlugins->list(effectList);
}

void CoreDriver::listEffectControls(const std::string &effectName,
                                    std::vector<std::string> &controlList) {
  ControlLister controlLister(controlList);
  controlLister.setEffectName(effectName);
  controlLister.visit(*processor);
}

void CoreDriver::setApi(const std::string &api) {
  (void) api;
}

void CoreDriver::setEffectControlValue(const std::string &effectName,
                                       const std::string &controlName,
                                       float value) {
  ControlUpdater controlUpdater;
  controlUpdater.setEffectName(effectName);
  controlUpdater.setControlName(controlName);
  controlUpdater.setControlValue(value);
  controlUpdater.visit(*processor);
}

void CoreDriver::start() {
  session->start();
}

void CoreDriver::stop() {
  session->stop();
}

} // namespace Qt

} // namespace GnuGuitar:

