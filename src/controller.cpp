#include <gnu-guitar-qt/controller.hpp>

#include <gnu-guitar-qt/ladspa-setup.hpp>
#include <gnu-guitar-qt/main-window.hpp>
#include <gnu-guitar-qt/preferences.hpp>

#include <rtaudio/api-specifier.hpp>
#include <rtaudio/composite-processor.hpp>
#include <rtaudio/ladspa-port.hpp>
#include <rtaudio/ladspa-processor.hpp>
#include <rtaudio/processor-visitor.hpp>

// TODO : remove
using namespace Gnuitar::Qt;

namespace {

class ControlUpdater final : public RtAudio::ProcessorVisitor {
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
  void visit(RtAudio::CompositeProcessor &compositeProcessor) {
    for (auto processor : compositeProcessor)
      processor->accept(*this);
  }
  void visit(RtAudio::LadspaProcessor &processor) {

    // TODO : throw exception if a control is not found

    if (processor.getName() != effectName)
      return;

    auto functor = [&](RtAudio::LadspaControl &control) {
      control.value = controlValue * (control.value_max - control.value_min);
    };

    RtAudio::LadspaControlFinder<decltype(functor)> controlFinder(controlName, functor);

    processor.acceptPortVisitor(controlFinder);
  }
};

} // namespace

namespace GnuGuitar {

namespace Qt {

Controller::Controller() {

  mainWindow = new MainWindow();

  mainWindow->resize(640, 480);
  mainWindow->show();

  mainWindow->connect(mainWindow, &MainWindow::effectChanged,
                      this, &Controller::onEffectChanged);
  mainWindow->connect(mainWindow, &MainWindow::effectClicked,
                      this, &Controller::addEffect);
  mainWindow->connect(mainWindow, &MainWindow::playClicked,
                      this, &Controller::onPlayClicked);
  mainWindow->connect(mainWindow, &MainWindow::preferencesClicked,
                      this, &Controller::onPreferencesClicked);
  mainWindow->connect(mainWindow, &MainWindow::stopClicked,
                      this, &Controller::onStopClicked);
  mainWindow->connect(mainWindow, &MainWindow::quitClicked,
                      this, &Controller::onQuitClicked);

  session.openRtApi(RtAudio::ApiSpecifier::ALSA);

  processor = new RtAudio::CompositeProcessor;
  session.setProcessor(processor);

  ladspaPlugins.addDefaultSearchPaths();
  updateEffectList();
}

Controller::~Controller() {
  if (mainWindow != nullptr) {
    delete mainWindow;
    mainWindow = nullptr;
  }
}

void Controller::addEffect(const QString &effect_name) {

  auto effect = new RtAudio::LadspaProcessor;
  if (!ladspaPlugins.find(effect_name.toStdString(), *effect)) {
    delete effect;
    // TODO : log this error
    return;
  }

  effect->instantiate(48000);
  effect->activate();

  std::vector<std::string> inputs;
  effect->listInputs(inputs);

  std::vector<std::string> outputs;
  effect->listOutputs(outputs);

  LadspaSetup ladspaSetup;
  for (const auto &input : inputs)
    ladspaSetup.addInput(input);
  for (const auto &output : outputs)
    ladspaSetup.addOutput(output);

  ladspaSetup.exec();

  if (ladspaSetup.cancelled()) {
    delete effect;
    return;
  }

  auto inputName = ladspaSetup.getInputName();
  auto outputName = ladspaSetup.getOutputName();

  effect->selectInput(inputName);
  effect->selectOutput(outputName);

  std::vector<std::string> controls;
  effect->listControls(controls);

  auto effectView = new EffectView(effect_name);

  for (const auto &control : controls)
    effectView->add_control(control.c_str());

  mainWindow->addEffect(effectView);

  processor->append(effect);
}

void Controller::onEffectChanged(const QString &effectName,
                                 const QString &controlName,
                                 int value) {

  float controlValue = 0.0f;
  controlValue += value;
  controlValue /= 100.0f;

  ControlUpdater controlUpdater;
  controlUpdater.setEffectName(effectName.toStdString());
  controlUpdater.setControlName(controlName.toStdString());
  controlUpdater.setControlValue(controlValue);

  processor->accept(controlUpdater);
}

void Controller::onPlayClicked() {

  RtAudio::StreamParameters iparams;
  RtAudio::StreamParameters oparams;
  RtAudio::StreamOptions options;

  unsigned int bufferFrames = 512;

  session.openStream(&oparams,
                     &iparams,
                     RtAudio::Format::Float32,
                     48000UL,
                     &bufferFrames,
                     &options);
  session.startStream();
}

void Controller::onPreferencesClicked() {
  Preferences preferences;
  preferences.exec();
}

void Controller::onStopClicked() {
  session.stopStream();
}

void Controller::onQuitClicked() {
  mainWindow->close();
}

void Controller::updateEffectList() {
  ladspaPlugins.update();

  auto effectList = ladspaPlugins.list();
  for (const auto &effectName : effectList)
    mainWindow->addEffectChoice(effectName.c_str());
}

} /* namespace Qt */

} /* namespace GnuGuitar */
