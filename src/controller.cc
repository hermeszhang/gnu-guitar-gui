#include <gnu-guitar-qt/controller.h>

#include <rtaudio/api-specifier.hpp>

namespace Gnuitar
{

namespace Qt
{

Controller::Controller (void)
{
  main_window.resize (640, 480);
  main_window.show ();

  main_window.connect (&main_window, &MainWindow::effect_changed, this, &Controller::on_effect_changed);
  main_window.connect (&main_window, &MainWindow::effect_selected, this, &Controller::add_effect);
  main_window.connect (&main_window, &MainWindow::play_selected, this, &Controller::on_play_selected);
  main_window.connect (&main_window, &MainWindow::stop_selected, this, &Controller::on_stop_selected);

  session.openRtApi(RtAudio::ApiSpecifier::ALSA);

  ladspa_plugins.addDefaultSearchPaths();
  update_effect_list ();
}

Controller::~Controller (void)
{
  session.stopStream();
}

void
Controller::add_effect (const QString& effect_name)
{
  /*
  auto effect = driver_manager.add_effect (effect_name.toStdString ());
  */

  auto effect_view = new EffectView (effect_name);

  /*
  auto control_set = effect->get_control_set ();
  for (const auto& control : control_set)
    effect_view->add_control (control.get_label ().c_str ());
  */

  main_window.add_effect (effect_view);
}

void
Controller::on_effect_changed (const QString& effect_name, const QString& control_name, int value)
{
  (void) effect_name;
  (void) control_name;
  (void) value;
/*
  driver_manager.set_control_value (effect_name.toStdString (),
                                    control_name.toStdString (),
                                    value);
 */
}

void
Controller::on_play_selected (void)
{
  /*
  driver_manager.start_driver ();
  */
}

void
Controller::on_stop_selected (void)
{
  /*
  driver_manager.stop_driver ();
  */
}

void
Controller::update_effect_list (void)
{
  ladspa_plugins.update();

  auto effect_list = ladspa_plugins.list();
  for (const auto& effect_name : effect_list)
    main_window.add_effect_choice (effect_name.c_str ());
}

} /* namespace Qt */

} /* namespace Gnuitar */

