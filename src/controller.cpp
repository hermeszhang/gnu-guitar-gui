#include <gnu-guitar-qt/controller.hpp>

#include <gnu-guitar-qt/ladspa-setup.hpp>
#include <gnu-guitar-qt/main-window.hpp>

#include <rtaudio/api-specifier.hpp>
#include <rtaudio/ladspa-processor.hpp>

// TODO : remove
using namespace Gnuitar::Qt;

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
  mainWindow->connect(mainWindow, &MainWindow::stopClicked,
                      this, &Controller::onStopClicked);
  mainWindow->connect(mainWindow, &MainWindow::quitClicked,
                      this, &Controller::onQuitClicked);

  session.openRtApi(RtAudio::ApiSpecifier::ALSA);

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

  session.setProcessor(effect);
}

void Controller::onEffectChanged(const QString &effect_name,
                                 const QString &control_name,
                                 int value) {
  (void)effect_name;
  (void)control_name;
  (void)value;
}

void Controller::onPlayClicked() {

  RtAudio::StreamParameters iparams;
  RtAudio::StreamParameters oparams;
  RtAudio::StreamOptions options;

  unsigned int bufferFrames = 512;

  session.openStream(&oparams,
                     &iparams,
                     RTAUDIO_FLOAT32,
                     48000UL,
                     &bufferFrames,
                     &options);
  session.startStream();
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
