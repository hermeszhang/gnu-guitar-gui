#include <gnu-guitar-qt/controller.hpp>

#include <gnu-guitar-qt/ladspa-setup.hpp>

#include <rtaudio/api-specifier.hpp>
#include <rtaudio/ladspa-processor.hpp>

// TODO : remove
using namespace Gnuitar::Qt;

namespace GnuGuitar {

namespace Qt {

Controller::Controller(void) {
  main_window.resize(640, 480);
  main_window.show();

  main_window.connect(&main_window, &MainWindow::effect_changed, this,
                      &Controller::on_effect_changed);
  main_window.connect(&main_window, &MainWindow::effect_selected, this,
                      &Controller::add_effect);
  main_window.connect(&main_window, &MainWindow::play_selected, this,
                      &Controller::on_play_selected);
  main_window.connect(&main_window, &MainWindow::stop_selected, this,
                      &Controller::on_stop_selected);

  session.openRtApi(RtAudio::ApiSpecifier::ALSA);

  ladspa_plugins.addDefaultSearchPaths();
  update_effect_list();
}

Controller::~Controller(void) {}

void Controller::add_effect(const QString &effect_name) {

  auto effect = new RtAudio::LadspaProcessor;
  if (!ladspa_plugins.find(effect_name.toStdString(), *effect)) {
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

  auto effect_view = new EffectView(effect_name);

  for (const auto &control : controls)
    effect_view->add_control(control.c_str());

  main_window.add_effect(effect_view);

  session.setProcessor(effect);
}

void Controller::on_effect_changed(const QString &effect_name,
                                   const QString &control_name, int value) {
  (void)effect_name;
  (void)control_name;
  (void)value;
}

void Controller::on_play_selected(void) {

  RtAudio::StreamParameters iparams;
  RtAudio::StreamParameters oparams;
  RtAudio::StreamOptions options;

  unsigned int buffer_frames = 512;

  session.openStream(&oparams,
                     &iparams,
                     RTAUDIO_FLOAT32,
                     48000UL,
                     &buffer_frames,
                     &options);
  session.startStream();
}

void Controller::on_stop_selected(void) {
  session.stopStream();
}

void Controller::update_effect_list(void) {
  ladspa_plugins.update();

  auto effect_list = ladspa_plugins.list();
  for (const auto &effect_name : effect_list)
    main_window.add_effect_choice(effect_name.c_str());
}

} /* namespace Qt */

} /* namespace GnuGuitar */
