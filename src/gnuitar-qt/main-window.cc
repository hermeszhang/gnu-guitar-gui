#include "main-window.h"

namespace Gnuitar {

namespace Qt {

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
  menu_bar = new MenuBar ();

  audio_panel = new AudioPanel ();
  audio_panel->resize (640, 120);

  rack = new Rack ();

  layout = new QVBoxLayout;
  layout->addWidget (audio_panel);
  layout->addWidget (rack);

  central_widget = new QWidget (this);
  central_widget->setLayout (layout);

  setCentralWidget (central_widget);
  setMenuBar (menu_bar);

  connect (menu_bar, &MenuBar::ladspa_plugin_selected, this, &MainWindow::ladspa_plugin_selected);
  connect (menu_bar, &MenuBar::quit_requested, this, &MainWindow::close);
  connect (audio_panel, &AudioPanel::play_triggered, this, &MainWindow::on_play_triggered);
  connect (audio_panel, &AudioPanel::stop_triggered, this, &MainWindow::on_stop_triggered);

  driver_manager.update_libs ();

  auto effect_list = driver_manager.get_effect_list ();
  for (const auto& effect_name : effect_list)
    {
      menu_bar->add_ladspa_plugin (effect_name.c_str ());
    }
}

MainWindow::~MainWindow (void)
{

}

void
MainWindow::on_play_triggered (void)
{
  driver_manager.stop_driver ();
}

void
MainWindow::on_stop_triggered (void)
{
  driver_manager.start_driver ();
}

void
MainWindow::ladspa_plugin_selected (const QString& qt_plugin_name)
{
  auto effect = driver_manager.add_effect (qt_plugin_name.toStdString ());

  auto effect_view = new EffectView (effect, rack);

  rack->add_effect (effect_view);
}

} /* namespace Qt */

} /* namespace Gnuitar */

