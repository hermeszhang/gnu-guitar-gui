#include <gnu-guitar/gui/core-driver.hpp>

#include <gnu-guitar/gui/api-settings.hpp>
#include <gnu-guitar/gui/binary-control.hpp>
#include <gnu-guitar/gui/control-visitor.hpp>
#include <gnu-guitar/gui/string-control.hpp>

#include <gnu-guitar-qt/ladspa-setup.hpp>
#include <gnu-guitar/qt/error-dialog.hpp>

#include <gnu-guitar-core/alsa/api.hpp>
#include <gnu-guitar-core/alsa/options.hpp>
#include <gnu-guitar-core/alsa/device-info.hpp>
#include <gnu-guitar-core/alsa/device-list.hpp>
#include <gnu-guitar-core/composite-processor.hpp>
#include <gnu-guitar-core/device-visitor.hpp>
#include <gnu-guitar-core/error.hpp>
#include <gnu-guitar-core/ladspa-plugins.hpp>
#include <gnu-guitar-core/ladspa-port.hpp>
#include <gnu-guitar-core/ladspa-processor.hpp>
#include <gnu-guitar-core/missing-device.hpp>
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

class DeviceLister final : public GnuGuitar::Core::DeviceVisitor {
  std::vector<std::string> &list;
public:
  DeviceLister(std::vector<std::string> &list_) : list(list_) {

  }
  ~DeviceLister() {

  }
  void visit(const GnuGuitar::Core::Alsa::DeviceInfo &deviceInfo) {
    list.push_back(deviceInfo.getName());
  }
  void visit(const GnuGuitar::Core::Jack::DeviceInfo &deviceInfo) {
    (void) deviceInfo;
  }
};

class AlsaOptionBuilder final : public GnuGuitar::Gui::ControlVisitor {
  GnuGuitar::Core::Alsa::Options &options;
public:
  AlsaOptionBuilder(GnuGuitar::Core::Alsa::Options &options_) : options(options_) {

  }
  ~AlsaOptionBuilder() {

  }
  void visit(const GnuGuitar::Gui::BinaryControl &binaryControl) {
    (void) binaryControl;
  }
  void visit(const GnuGuitar::Gui::StringControl &stringControl) {
    std::string controlName;
    stringControl.getName(controlName);
    if (controlName == "Input") {
      std::string controlValue;
      stringControl.getValue(controlValue);
      options.setInput(controlValue);
    } else if (controlName == "Output") {
      std::string controlValue;
      stringControl.getValue(controlValue);
      options.setOutput(controlValue);
    }
  }
};

void setAlsaSettings(GnuGuitar::Gui::ApiSettings &apiSettings) {

  GnuGuitar::Core::Alsa::Api alsaApi;

  GnuGuitar::Gui::StringControl inputControl;
  inputControl.setName("Input");

  GnuGuitar::Core::Alsa::DeviceList inputList;
  alsaApi.listInputs(inputList);
  for (const auto input : inputList)
    inputControl.addOption(input.getName());

  GnuGuitar::Core::Alsa::DeviceList outputList;
  alsaApi.listOutputs(outputList);

  GnuGuitar::Gui::StringControl outputControl;
  outputControl.setName("Output");
  for (const auto &output : outputList)
    outputControl.addOption(output.getName());

  apiSettings.setApiName("ALSA");
  apiSettings.addControl(inputControl);
  apiSettings.addControl(outputControl);
}

void setJackSettings(GnuGuitar::Gui::ApiSettings &apiSettings) {

  // TODO : this should be an edit field
  GnuGuitar::Gui::StringControl clientNameControl;
  clientNameControl.setName("Client Name");

  GnuGuitar::Gui::BinaryControl autoStartControl;
  autoStartControl.setName("Auto Start");

  apiSettings.setApiName("Jack");
  apiSettings.addControl(clientNameControl);
  apiSettings.addControl(autoStartControl);
}

} // namespace

namespace GnuGuitar {

namespace Gui {

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

void CoreDriver::listApis(std::vector<ApiSettings> &apiList) {
  std::vector<Core::ApiSpecifier> apiSpecifierList;
  Core::listCompiledApis(apiSpecifierList);
  for (auto apiSpecifier : apiSpecifierList) {
    ApiSettings apiSettings;
    switch (apiSpecifier) {
      case Core::ApiSpecifier::Alsa:
        setAlsaSettings(apiSettings);
        break;
      case Core::ApiSpecifier::Jack:
        setJackSettings(apiSettings);
        break;
      default:
        continue;
    }
    apiList.emplace_back(std::move(apiSettings));
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

void CoreDriver::setApi(const ApiSettings &apiSettings) {

  std::string apiName;
  apiSettings.getApiName(apiName);

  if (apiName == "ALSA") {

    GnuGuitar::Core::Alsa::Options alsaOptions;

    AlsaOptionBuilder optionBuilder(alsaOptions);

    apiSettings.accept(optionBuilder);

    session->setOptions(alsaOptions);
  }
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
  try {
    session->start();
  } catch (const GnuGuitar::Core::MissingDevice &missingDevice) {
    Qt::ErrorDialog errorDialog(missingDevice);
    errorDialog.exec();
  } catch (const GnuGuitar::Core::Error &error) {
    Qt::ErrorDialog errorDialog(error);
    errorDialog.exec();
  }
}

void CoreDriver::stop() {
  session->stop();
}

} // namespace Gui

} // namespace GnuGuitar:

